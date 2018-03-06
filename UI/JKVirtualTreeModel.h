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
	
	QVariant data(const QModelIndex &index, int role) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex &index) const override;
	bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;

	virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;
	
	BaseObjectPtr getItem(const QModelIndex &index) const;
	QModelIndex getItemIndex(BaseObjectConstRefPtr item) const;
	
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
	InternalNode *getItemNode(BaseObjectConstRefPtr item) const;
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

#endif // VIRTUALTREEADAPTER_H
