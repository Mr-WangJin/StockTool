#ifndef VIRTUALTREEADAPTER_H
#define VIRTUALTREEADAPTER_H

#include <QAbstractItemModel>
#include <memory>
#include "JKVirtualTreeAdapter.h"

class InternalNode;

class JKVirtualModelInterfaceImpl;

class JKVirtualTreeModel : public QAbstractItemModel
{
  Q_OBJECT
public:
  JKVirtualTreeModel(JKVirtualModelAdapter *adapter, QObject *parent = 0);
  ~JKVirtualTreeModel();

  QVariant data(const QModelIndex &index, int role) const override;
  QModelIndex index(int row, int column,
    const QModelIndex &parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex &index) const override;
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;

  BaseObjectConstRefPtr getItem(const QModelIndex &index) const;
  QModelIndex getItemIndex(BaseObjectConstRefPtr item) const;

  JKVirtualModelAdapter * setModelAdapter(JKVirtualModelAdapter *adapter);
  JKVirtualModelAdapter * getModelAdapter() const;

  void beginUpdate();
  void endUpdate();
  // tree is updating that means items in internal nodes may be invalid
  bool isUpdating() const;  

  void QueuedUpdate();

private:

  JKVirtualModelAdapter *m_adapter;
  JKVirtualModelInterfaceImpl *m_intf;
  // we need mutable nodes to allow lazy loading
  mutable InternalNode *m_root;
  int m_updating;

  InternalNode & getNode(const QModelIndex &index) const;
  InternalNode *getItemNode(BaseObjectConstRefPtr item) const;
  QModelIndex getIndex(const InternalNode &node, int column = 0) const;

  void syncNodeList(InternalNode &node, BaseObjectConstRefPtr parent);
  bool m_syncing;
  void syncTree();

private slots:
  void doQueuedUpdate();
};

#endif // VIRTUALTREEADAPTER_H
