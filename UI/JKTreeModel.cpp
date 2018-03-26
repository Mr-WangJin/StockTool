#include "stdafx.h"
#include "JKTreeModel.h"
#include "JKTreeModelItem.h"

JKTreeModel::JKTreeModel(QObject *parent):
    QAbstractItemModel(parent),
    rootItem(new JKTreeModelItem(0)){}

int JKTreeModel::columnCount(const QModelIndex &parent) const
{
    return getItem(parent)->columnCount();
}

QVariant JKTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    return getItem(index)->data(index.column(), role);
}

bool JKTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
        return false;

    return getItem(index)->setData(index.column(), value, role);
}

QModelIndex JKTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!hasIndex(row, column, parent))
        return QModelIndex();

    JKTreeModelItem *parentItem = getItem(parent);
    JKTreeModelItem *childItem = parentItem->child(row);

    return createIndex(row, column, childItem);
}

QModelIndex JKTreeModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    JKTreeModelItem *childItem = getItem(child);
    if(!childItem)
        return QModelIndex();
    JKTreeModelItem *parentItem = childItem->parent();

    if (!parentItem || parentItem == rootItem)
        return QModelIndex();

    JKTreeModelItem *grandParent = parentItem->parent();
    if(!grandParent)
        return QModelIndex();

    return createIndex(grandParent->childPosition(parentItem), 0, parentItem);
}

int JKTreeModel::rowCount(const QModelIndex &parent) const
{
    return getItem(parent)->childCount();
}

Qt::ItemFlags JKTreeModel::flags(const QModelIndex &index) const
{
    return getItem(index)->flags(index.column());
}

QVariant JKTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal)
        return rootItem->data(section, role);
    return QAbstractItemModel::headerData(section, orientation, role);
}

void JKTreeModel::setRootItem(JKTreeModelItem *rootItem)
{
    beginResetModel();
    this->rootItem.reset(rootItem);
    rootItem->propagateModel(this);
    endResetModel();
}

const JKTreeModelItem *JKTreeModel::getRootItem() const
{
    return rootItem.data();
}

JKTreeModelItem *JKTreeModel::getRootItem()
{
    return rootItem.data();
}

QModelIndex JKTreeModel::indexFromItem(const JKTreeModelItem *item) const
{
    if(!item || item == rootItem.data())
        return QModelIndex();

    return createIndex(item->parent()->childPosition(item), 0, const_cast<JKTreeModelItem *>(item));
}

JKTreeModelItem *JKTreeModel::itemFromIndex(const QModelIndex &index)
{
    return index.isValid()? getItem(index): 0;
}

const JKTreeModelItem *JKTreeModel::itemFromIndex(const QModelIndex &index) const
{
    return index.isValid()? getItem(index): 0;
}

void JKTreeModel::sort(int column, Qt::SortOrder order)
{
    rootItem->sort(column, order);
}

JKTreeModelItem *JKTreeModel::getItem(const QModelIndex &index) const
{
    return index.isValid() ?
                static_cast<JKTreeModelItem*>(index.internalPointer()) :
                rootItem.data();
}


bool JKTreeModel::removeRows(int row, int count, const QModelIndex &parent)
{
    JKTreeModelItem *parentItem;

    if (parent.isValid()) {
        parentItem = itemFromIndex(parent);
    } else {
        parentItem = getRootItem();
    }

    if (!(row < parentItem->childCount() && (row + count) <= parentItem->childCount())) {
        return false;
    }

    bool result = parentItem->removeChildren(row, count);
    parentItem->updateRecursive();

    return result;
}
