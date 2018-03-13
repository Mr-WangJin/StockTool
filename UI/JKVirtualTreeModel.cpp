#include "stdafx.h"
#include "JKVirtualTreeModel.h"
#include "JKVirtualTreeAdapter.h"

class JKVirtualModelInterfaceImpl : public JKVirtualModelInterface
{
public:
	JKVirtualModelInterfaceImpl(JKVirtualTreeModel &model) : m_model(model) {};
	virtual ~JKVirtualModelInterfaceImpl() {};

	void beginUpdate() override
	{
		m_model.beginUpdate();
	}
	void endUpdate() override
	{
		m_model.endUpdate();
	}
	void QueuedUpdate() override
	{
		m_model.QueuedUpdate();
	}
private:
	JKVirtualTreeModel &m_model;
};

typedef std::vector<std::unique_ptr<InternalNode>> InternalChildren;

// internal tree structure
class InternalNode
{
public:
	InternalNode(InternalNode *parent, BaseObjectConstRefPtr obj, size_t index) : parent(parent), item(obj), parentIndex(index) {}
	InternalNode *parent;
	size_t parentIndex;  
	InternalChildren children;
	bool hasChildrenQueryed = false;
	bool hasChildren = false;
	bool childInitialized = false;
	BaseObjectPtr item;

	bool isInitialized(const std::shared_ptr<JKVirtualModelAdapter>& _modelAdapter)
	{
		if (!childInitialized)
		{
			// if node children have not been added to model
			// but model asks node by hasChildren()
			// the only way to notify model about changes is to initialize node children
			if (hasChildrenQueryed) {
				if (hasChildren != (_modelAdapter->getItemsCount(item) > 0))
					childInitialized = true;
			}
			return childInitialized;
		}
		else
			return true;
	}
	
	void loadChildren(const std::shared_ptr<JKVirtualModelAdapter>& _modelAdapter)
	{
		if (!childInitialized)
		{
			if (_modelAdapter->hasItems(item))
			{
				int childCount = _modelAdapter->getItemsCount(item);
				for (int k = 0; k < childCount; ++k)
					children.emplace_back(new InternalNode(this, _modelAdapter->getItem(item, k), k));
			}
			childInitialized = true;
		}
	}
	
	int childCount(const std::shared_ptr<JKVirtualModelAdapter>& _modelAdapter)
	{
		loadChildren(_modelAdapter);
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

JKVirtualTreeModel::JKVirtualTreeModel(const std::shared_ptr<JKVirtualModelAdapter>& _modelAdapter, QObject *parent) :
  QAbstractItemModel(parent), modelAdapter(_modelAdapter), m_updating(0), m_syncing(false)
{
	rootNode = new InternalNode(nullptr, nullptr, 0);
	m_intf = new JKVirtualModelInterfaceImpl(*this);
	if (modelAdapter)
		modelAdapter->setModel(m_intf);
	syncTree();
}

JKVirtualTreeModel::~JKVirtualTreeModel()
{
	delete rootNode;
	delete m_intf;
}

void JKVirtualTreeModel::syncNodeList(InternalNode &node, BaseObjectConstRefPtr parent)
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
	    destCur = modelAdapter->indexOf(parent, curNode->item, destStart);
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
	      destCur = modelAdapter->getItemsCount(parent);
	    // insert skipped new nodes
	    if (destCur > destStart)
	    {
	      int insertCount = destCur - destStart;
	      beginInsertRows(index, srcCur, srcCur + insertCount - 1);
	      for (int i = 0, cur = srcCur; i < insertCount; i++, cur++)
	      {
				BaseObjectConstRefPtr obj = modelAdapter->getItem(parent, destStart + i);
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

	    if (curNode && curNode->isInitialized(modelAdapter))
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

	if (m_updating > 0 || modelAdapter == nullptr)
	  return QVariant();

	BaseObjectPtr item = getNode(index).item;
	return modelAdapter->data(item, role, index);
}

QVariant JKVirtualTreeModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
	if (modelAdapter)
		return modelAdapter->headerData(section, orientation, role);

	return QVariant();
}

bool JKVirtualTreeModel::setData(const QModelIndex &index, const QVariant &value, int role /*= Qt::EditRole*/)
{
	if (!index.isValid() || !modelAdapter)
		return false;

	BaseObjectPtr item = getNode(index).item;
	return modelAdapter->setData(item, value, role);
}

Qt::ItemFlags JKVirtualTreeModel::flags(const QModelIndex & index) const
{
	if (!modelAdapter)
		return Qt::ItemFlag::NoItemFlags;

	return modelAdapter->flags(index);
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
	if (!index.isValid() || !modelAdapter)
	  return QModelIndex();

	InternalNode &childItem = getNode(index);
	InternalNode *parentItem = childItem.parent;
	if (parentItem == nullptr || parentItem == rootNode)
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
	else if (modelAdapter)
	  // lazy children loading
		return parentItem.childCount(modelAdapter);
	else
		return 0;
}

int JKVirtualTreeModel::columnCount(const QModelIndex &parent) const
{
	//Q_UNUSED(parent);

	if (modelAdapter)
		return modelAdapter->getColumnCount();

	return 0;
}

InternalNode & JKVirtualTreeModel::getNode(const QModelIndex &index) const
{
	if (index.isValid())
	  return *(InternalNode*)index.internalPointer();
	else
	  return *rootNode;
}

InternalNode *JKVirtualTreeModel::getItemNode(BaseObjectConstRefPtr item) const
{
	if (modelAdapter == nullptr)
	  return nullptr;
	BaseObjectConstRefPtr parentItem = modelAdapter->getItemParent(item);
	if (parentItem == item)
	  return nullptr;
	if (parentItem == nullptr)
	  return rootNode;
	else
	{
	  auto parentNode = getItemNode(parentItem);
	  int index = modelAdapter->indexOf(parentItem, item);
	  if (index >= 0)
	  {
	    parentNode->loadChildren(modelAdapter);
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
	if (&node == rootNode)
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
	InternalNode &node = getNode(parent);
	if (node.childInitialized)
	{
	  return node.children.size() > 0;
	}
	else if (modelAdapter)
	{
		  node.hasChildrenQueryed = true;
	  bool has = modelAdapter->hasItems(node.item);
		node.hasChildren = has;
	  return has;
	}
	else
	  return false;
}

void JKVirtualTreeModel::sort(int column, Qt::SortOrder order /*= Qt::AscendingOrder*/)
{

}

BaseObjectPtr JKVirtualTreeModel::getItem(const QModelIndex &index) const
{
	return getNode(index).item;
}

QModelIndex JKVirtualTreeModel::getItemIndex(BaseObjectConstRefPtr item) const
{
	auto node = getItemNode(item);
	if (node)
	  return getIndex(*node);
	else
	  return QModelIndex();
}

void JKVirtualTreeModel::setModelAdapter(const std::shared_ptr<JKVirtualModelAdapter>& _modelAdapter)
{
	if (modelAdapter != _modelAdapter || _modelAdapter->getRootChanged())
	{
		beginUpdate();
		if (modelAdapter)
			modelAdapter->setModel(nullptr);

		modelAdapter = _modelAdapter;

		beginResetModel();
		rootNode->children.clear();
		endResetModel();

		if (modelAdapter)
			modelAdapter->setModel(m_intf);
		endUpdate();
	}
	else
	{
		this->QueuedUpdate();
	}

	modelAdapter->setRootChanged(false);
}

const std::shared_ptr<JKVirtualModelAdapter>& JKVirtualTreeModel::getModelAdapter() const
{
	return modelAdapter;
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
	if (modelAdapter)
	{
		m_syncing = true;
		syncNodeList(*rootNode, nullptr);
		m_syncing = false;
	}
}
