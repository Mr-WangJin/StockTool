#include "stdafx.h"
#include "JKVirtualTreeModel.h"

class JKVirtualModelInterfaceImpl : public JKVirtualModelInterface
{
public:
  JKVirtualModelInterfaceImpl(JKVirtualTreeModel &model);
  void beginUpdate() override;
  void endUpdate() override;
  void QueuedUpdate() override;
private:
  JKVirtualTreeModel &m_model;
};


JKVirtualModelInterfaceImpl::JKVirtualModelInterfaceImpl(JKVirtualTreeModel &model) : m_model(model)
{

}

void JKVirtualModelInterfaceImpl::beginUpdate() {
  m_model.beginUpdate();
}

void JKVirtualModelInterfaceImpl::endUpdate() {
  m_model.endUpdate();
}

void JKVirtualModelInterfaceImpl::QueuedUpdate()
{
  m_model.QueuedUpdate();
}

typedef std::vector<std::unique_ptr<InternalNode>> InternalChildren;

// internal tree structure
class InternalNode
{
public:
  InternalNode(InternalNode *parent, void *obj, size_t index) : parent(parent), item(obj), parentIndex(index) {}
  InternalNode *parent;
  void *item;
  size_t parentIndex;  
  InternalChildren children;
  bool hasChildrenQueryed = false;
  bool hasChildren = false;
  bool childInitialized = false;

  bool isInitialized(JKVirtualModelAdapter *adapter)
  {
    if (!childInitialized)
    {
      // if node children have not been added to model
      // but model asks node by hasChildren()
      // the only way to notify model about changes is to initialize node children
      if (hasChildrenQueryed) {
        if (hasChildren != (adapter->getItemsCount(item) > 0))
          childInitialized = true;
      }
      return childInitialized;
    }
    else
      return true;
  }

  void loadChildren(JKVirtualModelAdapter *adapter)
  {
    if (!childInitialized)
    {
      if (adapter->hasItems(item))
      {
        int childCount = adapter->getItemsCount(item);
        for (int k = 0; k < childCount; ++k)
          children.emplace_back(new InternalNode(this, adapter->getItem(item, k), k));
      }
      childInitialized = true;
    }
  }

  int childCount(JKVirtualModelAdapter *adapter)
  {
    loadChildren(adapter);
    return static_cast<int>(children.size());
  }

  void eraseChildren(const InternalChildren::iterator &begin, const InternalChildren::iterator &end)
  {
    size_t curParent = begin->get()->parentIndex;
    auto newEnd = children.erase(begin, end);
    for (auto it = newEnd; it != children.end(); ++it)
      it->get()->parentIndex = curParent++;
  }

  void insertedChildren(size_t lastIndex)
  {
    for (auto index = lastIndex; index < children.size(); ++index)
      children[index]->parentIndex = index;
  }
};

JKVirtualTreeModel::JKVirtualTreeModel(JKVirtualModelAdapter *adapter, QObject *parent) :
  QAbstractItemModel(parent), m_adapter(adapter), m_updating(0), m_syncing(false)
{
  m_root = new InternalNode(nullptr, nullptr, 0);
  m_intf = new JKVirtualModelInterfaceImpl(*this);
  if (m_adapter)
    adapter->setModel(m_intf);
  syncTree();
}

JKVirtualTreeModel::~JKVirtualTreeModel()
{
  delete m_root;
  delete m_intf;
}

void JKVirtualTreeModel::syncNodeList(InternalNode &node, void *parent)
{
  InternalChildren &nodes = node.children;
  int srcStart = 0;
  int srcCur = srcStart;
  int destStart = 0;

  auto index = getIndex(node);
  while (srcCur <= static_cast<int>(nodes.size()))
  {
    bool finishing = srcCur >= static_cast<int>(nodes.size());
    int destCur = 0;
    InternalNode *curNode = nullptr;
    if (!finishing) {
      curNode = nodes[srcCur].get();
      destCur = m_adapter->indexOf(parent, curNode->item, destStart);
    }
    if (destCur >= 0)
    {
      // remove skipped source nodes
      if (srcCur > srcStart)
      {
        beginRemoveRows(index, srcStart, srcCur - 1);
        node.eraseChildren(nodes.begin() + srcStart, nodes.begin() + srcCur);
        if (!finishing)
          srcCur = srcStart;
        endRemoveRows();
      }
      srcStart = srcCur + 1;

      if (finishing)
        destCur = m_adapter->getItemsCount(parent);
      // insert skipped new nodes
      if (destCur > destStart)
      {
        int insertCount = destCur - destStart;
        beginInsertRows(index, srcCur, srcCur + insertCount - 1);
        for (int i = 0, cur = srcCur; i < insertCount; i++, cur++)
        {
          void *obj = m_adapter->getItem(parent, destStart + i);
          auto newNode = new InternalNode(&node, obj, cur);
          // just add new node we shouldn't sync its children yet
          nodes.emplace(nodes.begin() + cur, newNode);
        }
        node.insertedChildren(srcCur + insertCount);
        endInsertRows();

        srcCur += insertCount;
        destStart += insertCount;
      }
      destStart = destCur + 1;

      if (curNode && curNode->isInitialized(m_adapter))
      {
        syncNodeList(*curNode, curNode->item);
        srcStart = srcCur + 1;
      }
    }
    srcCur++;
  }
  node.childInitialized = true;
}

QVariant JKVirtualTreeModel::data(const QModelIndex &index, int role) const
{  
  if (!index.isValid())
    return QVariant();

  if (m_updating > 0 || m_adapter == nullptr)
    return QVariant();

  void *item = getNode(index).item;
  return m_adapter->data(item, role);
}

QModelIndex JKVirtualTreeModel::index(int row, int column, const QModelIndex &parent) const
{  
  InternalNode &parentItem = getNode(parent);
  if (row < static_cast<int>(parentItem.children.size()))
  {
    InternalNode *childItem = parentItem.children.at(row).get();
    return getIndex(*childItem, column);
  }
  else
    return QModelIndex();
}

QModelIndex JKVirtualTreeModel::parent(const QModelIndex &index) const
{
  if (!index.isValid() || !m_adapter)
    return QModelIndex();

  InternalNode &childItem = getNode(index);
  InternalNode *parentItem = childItem.parent;
  if (parentItem == nullptr || parentItem == m_root)
    return QModelIndex();
  return getIndex(*parentItem);
}

int JKVirtualTreeModel::rowCount(const QModelIndex &parent) const
{
  if (parent.isValid() && !this->parent(parent).isValid())
    parent.internalId();
  InternalNode &parentItem = getNode(parent);
  if (m_syncing)
    return static_cast<int>(parentItem.children.size());
  else if (m_adapter)
    // lazy children loading
    return parentItem.childCount(m_adapter);
  else
    return 0;
}

int JKVirtualTreeModel::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return 1;
}

InternalNode & JKVirtualTreeModel::getNode(const QModelIndex &index) const
{
  if (index.isValid())
    return *(InternalNode*)index.internalPointer();
  else
    return *m_root;
}

InternalNode *JKVirtualTreeModel::getItemNode(void *item) const
{
  if (m_adapter == nullptr)
    return nullptr;
  void * parentItem = m_adapter->getItemParent(item);
  if (parentItem == item)
    return nullptr;
  if (parentItem == nullptr)
    return m_root;
  else
  {
    auto parentNode = getItemNode(parentItem);
    int index = m_adapter->indexOf(parentItem, item);
    if (index >= 0)
    {
      parentNode->loadChildren(m_adapter);
      return parentNode->children[index].get();
    }
    else
    {
      return nullptr;
    }
  }
}

QModelIndex JKVirtualTreeModel::getIndex(const InternalNode &node, int column) const
{
  if (&node == m_root)
    return QModelIndex();
  else
    return createIndex(static_cast<int>(node.parentIndex), column, quintptr(&node));
}

void JKVirtualTreeModel::doQueuedUpdate()
{
  endUpdate();
}

bool JKVirtualTreeModel::hasChildren(const QModelIndex &parent) const
{
  if (!parent.isValid())
    return true;
  InternalNode &item = getNode(parent);
  if (item.childInitialized)
  {
    return item.children.size() > 0;
  }
  else if (m_adapter)
  {
    item.hasChildrenQueryed = true;
    bool has = m_adapter->hasItems(item.item);
    item.hasChildren = has;
    return has;
  }
  else
    return false;
}

void *JKVirtualTreeModel::getItem(const QModelIndex &index) const
{
  return getNode(index).item;
}

QModelIndex JKVirtualTreeModel::getItemIndex(void *item) const
{
  auto node = getItemNode(item);
  if (node)
    return getIndex(*node);
  else
    return QModelIndex();
}

JKVirtualModelAdapter * JKVirtualTreeModel::setModelAdapter(JKVirtualModelAdapter *adapter)
{
  JKVirtualModelAdapter *oldAdapter = m_adapter;
  beginUpdate();
  if (m_adapter)
    m_adapter->setModel(nullptr);
  m_adapter = adapter;
  beginResetModel();
  m_root->children.clear();
  endResetModel();
  if (m_adapter)  
    m_adapter->setModel(m_intf);
  endUpdate();
  return oldAdapter;
}

JKVirtualModelAdapter *JKVirtualTreeModel::getModelAdapter() const
{
  return m_adapter;
}

void JKVirtualTreeModel::beginUpdate()
{
  ++m_updating;
}

void JKVirtualTreeModel::endUpdate()
{
  if (m_updating == 1)
    syncTree();
  --m_updating;
  // force tree to repaint all nodes
  if (m_updating == 0)
    emit dataChanged(QModelIndex(), QModelIndex());
}

bool JKVirtualTreeModel::isUpdating() const
{
  return m_updating > 0 || m_syncing;
}

void JKVirtualTreeModel::QueuedUpdate()
{
  if (m_updating == 0)
  {
    beginUpdate();
    QMetaObject::invokeMethod(this, "doQueuedUpdate", Qt::QueuedConnection);
  }
}

void JKVirtualTreeModel::syncTree()
{
  if (m_adapter)
  {
    m_syncing = true;
    syncNodeList(*m_root, nullptr);
    m_syncing = false;
  }
}
