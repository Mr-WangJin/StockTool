#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QSharedPointer>

class JKTreeModelItem;

class JKTreeModel: public QAbstractItemModel
{
public:
    explicit				JKTreeModel(QObject *parent = 0);

    int						columnCount(const QModelIndex &parent) const override;
    QVariant				data(const QModelIndex &index, int role) const override;
    bool					setData(const QModelIndex &index, const QVariant &value, int role) override;
    QModelIndex				index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex				parent(const QModelIndex &child) const override;
    int						rowCount(const QModelIndex &parent) const override;
    Qt::ItemFlags			flags(const QModelIndex &index) const override;
    QVariant				headerData(int section, Qt::Orientation orientation, int role) const override;
    virtual bool			removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    void					setRootItem(JKTreeModelItem *rootItem);
    const JKTreeModelItem	*getRootItem() const;
    JKTreeModelItem			*getRootItem();
    QModelIndex				indexFromItem(const JKTreeModelItem *item) const;
    JKTreeModelItem			*itemFromIndex(const QModelIndex &index);
    const JKTreeModelItem	*itemFromIndex(const QModelIndex &index) const;


    virtual void			sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;

private:
    QSharedPointer<JKTreeModelItem>   rootItem;
    JKTreeModelItem                   *getItem(const QModelIndex &index) const;

    friend class JKTreeModelItem;
};

#endif // TREEMODEL_H
