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
	/** 进入该函数有两张情况
	 *  1. 当node的child为空的时候，需要重新建立node结构的
	 *  2. 不需要重新建立node结构，需要更新数据
	 */
	InternalChildren &childNodes = node.children;
	int nodeStartIndex = 0;
	int nodeCurIndex = 0;
	int objectStartIndex = 0;

	auto index = getIndex(node);
	while (nodeCurIndex <= static_cast<int>(childNodes.size()))
	{
		bool finishing = nodeCurIndex >= static_cast<int>(childNodes.size());
		int objectIndex = 0;
		InternalNode *curNode = nullptr;
		if (!finishing) 
		{
			curNode = childNodes[nodeCurIndex].get();
			objectIndex = modelAdapter->indexOf(parent, curNode->item, objectStartIndex);
		}
		if (objectIndex >= 0)
		{
			// 移除节点
			if (nodeCurIndex > nodeStartIndex)
			{
				beginRemoveRows(index, nodeStartIndex, nodeCurIndex - 1);
				node.eraseChildren(childNodes.begin() + nodeStartIndex, childNodes.begin() + nodeCurIndex);
				if (!finishing)
					nodeCurIndex = nodeStartIndex;
				endRemoveRows();
			}
			nodeStartIndex = nodeCurIndex + 1;

			if (finishing)
				objectIndex = modelAdapter->getChildItemsCount(parent);
			// 插入节点
			if (objectIndex > objectStartIndex)
			{
				int insertCount = objectIndex - objectStartIndex;
				beginInsertRows(index, nodeCurIndex, nodeCurIndex + insertCount - 1);
				for (int i = 0, cur = nodeCurIndex; i < insertCount; i++, cur++)
				{
					BaseObjectConstRefPtr obj = modelAdapter->getChildItem(parent, objectStartIndex + i);
					auto newNode = new InternalNode(&node, obj, cur);
					// 只是添加新节点，不同步它的孩子节点
					childNodes.emplace(childNodes.begin() + cur, newNode);
				}
				node.insertedChildren(nodeCurIndex + insertCount);
				endInsertRows();

				nodeCurIndex += insertCount;
				objectStartIndex += insertCount;
			}
			objectStartIndex = objectIndex + 1;

			if (curNode && curNode->isInitialized(modelAdapter))
			{
				syncNodeList(*curNode, curNode->item);
				nodeStartIndex = nodeCurIndex + 1;
			}
		}
		nodeCurIndex++;
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
	Q_UNUSED(parent);

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

// InternalNode *JKVirtualTreeModel::getItemNode(BaseObjectConstRefPtr item) const
// {
// 	/** 通过业务对象获取InternalNode对象。
// 	 *  1. 判断Adapter是否为空，空值返回nullptr
// 	 *  2. 获取item的父object，如果父object 等于 item返回空，如果父object为空，返回更节点。
// 	 *  3. 
// 	 */
// 	if (modelAdapter == nullptr)
// 		return nullptr;
// 	BaseObjectConstRefPtr parentItem = modelAdapter->getItemParent(item);
// 	if (parentItem == item)
// 		return nullptr;
// 	if (parentItem == nullptr)
// 		return rootNode;
// 	else
// 	{
// 		auto parentNode = getItemNode(parentItem);
// 		int index = modelAdapter->indexOf(parentItem, item);
// 		if (index >= 0)
// 		{
// 			parentNode->loadChild(modelAdapter);
// 			return parentNode->children[index].get();
// 		}
// 		else
// 		{
// 			return nullptr;
// 		}
// 	}
// }

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
	//QAbstractItemModel::sort(column, order);
	//rootNode->sortChildren(modelAdapter, column, order);

// 	QVector<int> roles;
// 	roles << Qt::DisplayRole;
// 	roles << Qt::BackgroundColorRole;
// 	emit dataChanged(QModelIndex(), QModelIndex(), roles);
	this->QueuedUpdate();
}

BaseObjectPtr JKVirtualTreeModel::getItem(const QModelIndex &index) const
{
	return getNode(index).item;
}

// QModelIndex JKVirtualTreeModel::getItemIndex(BaseObjectConstRefPtr item) const
// {
// 	auto node = getItemNode(item);
// 	if (node)
// 		return getIndex(*node);
// 	else
// 		return QModelIndex();
// }

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

//////////////////////////////////////////////////////////////////////////

InternalNode::~InternalNode()
{
}

bool InternalNode::isInitialized(const std::shared_ptr<JKVirtualModelAdapter>& _modelAdapter)
{
	/** 同步Adapter是否有子节点 */
	if (!childInitialized)
	{
		if (hasChildrenQueryed) {
			if (hasChildren != (_modelAdapter->getChildItemsCount(item) > 0))
				childInitialized = true;
		}
		return childInitialized;
	}
	else
		return true;
}

void InternalNode::loadChild(const std::shared_ptr<JKVirtualModelAdapter>& _modelAdapter)
{
	if (!childInitialized)
	{
		if (_modelAdapter->hasItems(item))
		{
			int childCount = _modelAdapter->getChildItemsCount(item);
			for (int k = 0; k < childCount; ++k)
				children.emplace_back(new InternalNode(this, _modelAdapter->getChildItem(item, k), k));
		}
		childInitialized = true;
	}
}

int InternalNode::childCount(const std::shared_ptr<JKVirtualModelAdapter>& _modelAdapter)
{
	loadChild(_modelAdapter);
	return static_cast<int>(children.size());
}

void InternalNode::eraseChildren(const InternalChildren::iterator & begin, const InternalChildren::iterator & end)
{
	size_t curParent = begin->get()->parentIndex;
	auto newEnd = children.erase(begin, end);
	for (auto it = newEnd; it != children.end(); ++it)
		it->get()->parentIndex = curParent++;
}

void InternalNode::insertedChildren(size_t lastIndex)
{
	for (auto index = lastIndex; index < children.size(); ++index)
		children[index]->parentIndex = index;
}

void InternalNode::sortChildren(const std::shared_ptr<JKVirtualModelAdapter>& adapter, int column, Qt::SortOrder order)
{
	if (!adapter)
		return;

	for each (auto &var in children)
	{
		var->sortChildren(adapter, column, order);
	}
	if (order == Qt::SortOrder::DescendingOrder)
	{


// 		std::sort(children.begin(), children.end(), [&](const std::unique_ptr<InternalNode> &a, const std::unique_ptr<InternalNode> &b)
// 		{
// 			return adapter->data(a->item, Qt::DisplayRole, 0, column) > adapter->data(b->item, Qt::DisplayRole, 0, column);
// 		});
	}
	else
	{
// 		std::sort(children.begin(), children.end(), [&](const std::unique_ptr<InternalNode> &a, const std::unique_ptr<InternalNode> &b)
// 		{
// 			return adapter->data(a->item, Qt::DisplayRole, 0, column) < adapter->data(b->item, Qt::DisplayRole, 0, column);
// 		});
	}
}