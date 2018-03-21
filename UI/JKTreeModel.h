#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QSharedPointer>

class JKTreeModelItem;

class JKTreeModel: public QAbstractItemModel
{
public:
    explicit        JKTreeModel(QObject *parent = 0);

    int             columnCount(const QModelIndex &parent) const;
    QVariant        data(const QModelIndex &index, int role) const;
    bool            setData(const QModelIndex &index, const QVariant &value, int role);
    QModelIndex     index(int row, int column, const QModelIndex &parent) const;
    QModelIndex     parent(const QModelIndex &child) const;
    int             rowCount(const QModelIndex &parent) const;
    Qt::ItemFlags   flags(const QModelIndex &index) const;
    QVariant        headerData(int section, Qt::Orientation orientation, int role) const;
    virtual bool    removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    void            setRootItem(JKTreeModelItem *rootItem);
    const JKTreeModelItem *getRootItem() const;
    JKTreeModelItem   *getRootItem();
    QModelIndex     indexFromItem(const JKTreeModelItem *item) const;
    JKTreeModelItem   *itemFromIndex(const QModelIndex &index);
    const JKTreeModelItem
                    *itemFromIndex(const QModelIndex &index) const;


    virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;

private:
    QSharedPointer<JKTreeModelItem>   rootItem;
    JKTreeModelItem                   *getItem(const QModelIndex &index) const;

    friend class JKTreeModelItem;
};

#endif // TREEMODEL_H
