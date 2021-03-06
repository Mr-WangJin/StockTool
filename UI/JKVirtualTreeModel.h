#ifndef VIRTUALTREEADAPTER_H
#define VIRTUALTREEADAPTER_H

#include <QAbstractItemModel>
#include <memory>

class JKVirtualModelAdapter;
class InternalNode;
class JKVirtualModelInterfaceImpl;

class JKVirtualTreeModel : public QAbstractItemModel
{
	Q_OBJECT
public:
	JKVirtualTreeModel(const std::shared_ptr<JKVirtualModelAdapter>& _modelAdapter, QObject *parent = 0);
	~JKVirtualTreeModel();
	
	virtual QVariant data(const QModelIndex &index, int role) const override;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const override;

	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
	virtual QModelIndex parent(const QModelIndex &index) const override;
	virtual bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;

	virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;
	
	BaseObjectPtr getItem(const QModelIndex &index) const;
	//QModelIndex getItemIndex(BaseObjectConstRefPtr item) const;
	
	void setModelAdapter(const std::shared_ptr<JKVirtualModelAdapter>& _modelAdapter);
	const std::shared_ptr<JKVirtualModelAdapter>& getModelAdapter() const;
	
	void beginUpdate();
	void endUpdate();
	// tree is updating that means items in internal nodes may be invalid
	bool isUpdating() const;  
	
	void QueuedUpdate();

private slots:
	void doQueuedUpdate();

private:
	InternalNode & getNode(const QModelIndex &index) const;
	//InternalNode *getItemNode(BaseObjectConstRefPtr item) const;
	QModelIndex getIndex(const InternalNode &node, int column = 0) const;

	void syncNodeList(InternalNode &node, BaseObjectConstRefPtr parent);
	void syncTree();

private:

	std::shared_ptr<JKVirtualModelAdapter> modelAdapter;
	JKVirtualModelInterfaceImpl *m_intf;
	mutable InternalNode *rootNode = nullptr;

	int m_updating;
	bool m_syncing;
};


typedef std::vector<std::unique_ptr<InternalNode>> InternalChildren;

// internal tree structure
class InternalNode
{
public:
	InternalNode(InternalNode *parent, BaseObjectConstRefPtr obj, size_t index) : parent(parent), item(obj), parentIndex(index) {}
	~InternalNode();

	bool isInitialized(const std::shared_ptr<JKVirtualModelAdapter>& _modelAdapter);
	
	void loadChild(const std::shared_ptr<JKVirtualModelAdapter>& _modelAdapter);

	int childCount(const std::shared_ptr<JKVirtualModelAdapter>& _modelAdapter);

	void eraseChildren(const InternalChildren::iterator &begin, const InternalChildren::iterator &end);

	void insertedChildren(size_t lastIndex);

	void moveHere(InternalNode* item, int pos);

	void sortChildren(const std::shared_ptr<JKVirtualModelAdapter> &adapter, int column, Qt::SortOrder order);
	
	int getParentItemIndex(BaseObjectConstRefPtr item)
	{
		if (parent == nullptr)
			return 0;
		for (int i = 0; i< parent->children.size(); ++i)
		{
			if (parent->children[i].get() == this)
			{
				return i;
			}
		}
	};

	InternalNode *parent;
	InternalChildren children;
	bool hasChildrenQueryed = false;
	bool hasChildren = false;
	bool childInitialized = false;

	BaseObjectPtr item;

	size_t parentIndex;


};



#endif // VIRTUALTREEADAPTER_H
