#include "stdafx.h"
#include "JKTreeModelItem.h"
#include "JKTreeModel.h"

JKTreeModelItem::JKTreeModelItem(int columns):
    m_column_count(columns) {}

JKTreeModelItem *JKTreeModelItem::child(int number)
{
    return const_cast<JKTreeModelItem *>(static_cast<const JKTreeModelItem &>(*this).child(number));
}

const JKTreeModelItem *JKTreeModelItem::child(int number) const
{
    return children.value(number).data();
}

int JKTreeModelItem::childCount() const
{
    return children.size();
}

int JKTreeModelItem::childPosition(const JKTreeModelItem *item) const
{
    int result = -1;
    for(int i = 0; i < children.size(); ++i)
    {
        if(children[i].data() == item)
        {
            result = i;
            break;
        }
    }
    return result;
}

JKTreeModelItem *JKTreeModelItem::parent()
{
    return m_parent;
}

const JKTreeModelItem *JKTreeModelItem::parent() const
{
    return m_parent;
}

bool JKTreeModelItem::removeChildren(int position, int count)
{
    if(position + count > children.size())
        return false;

    if(count == 0)
        return true;

    if(m_model)
    {
        QModelIndex i = index();
        m_model->beginRemoveRows(i, position, position + count - 1);
    }

    children.erase(children.begin() + position,
                   children.begin() + position + count);

    if(m_model)
        m_model->endRemoveRows();

    return true;
}

void JKTreeModelItem::removeChildren()
{
    removeChildren(0, children.size());
}

void JKTreeModelItem::moveHere(TreeModelItemPtr item, int pos)
{
    JKTreeModelItem *oldParent = item->parent();
    int oldPos = oldParent->childPosition(item.data());

    oldParent->removeChildren(oldPos, 1);

    if (oldParent == this && pos > oldPos) {
        pos--;
    }

    insertChild(pos, item);

    updateRecursive();
}

TreeModelItemPtr JKTreeModelItem::takeChild(int number)
{
    if (number < 0 || number > children.size()) {
        throw std::range_error("JKTreeModelItem::takeChild - Invalid index");
    }

    return children.at(number);
}

void JKTreeModelItem::detachChild(TreeModelItemPtr ptr)
{
    children.removeOne(ptr);
}

Qt::ItemFlags JKTreeModelItem::flags(int column) const
{
    Q_UNUSED(column);

    return Qt::ItemIsEnabled;
}

void JKTreeModelItem::prependChild(JKTreeModelItem *childItem)
{
    insertChild(0, childItem);
}

void JKTreeModelItem::appendChild(JKTreeModelItem *childItem)
{
    insertChild(children.size(), childItem);
}

void JKTreeModelItem::insertChild(int pos, JKTreeModelItem *childItem)
{
    insertChild(pos, TreeModelItemPtr(childItem));
}

void JKTreeModelItem::insertChild(int pos, TreeModelItemPtr childItem)
{
    pos = std::min(children.size(), pos);
    pos = std::max(0, pos);

    if(m_model)
    {
        QModelIndex i = index();
        m_model->beginInsertRows(i, pos, pos);
    }

    childItem->setParent(this);
    children.insert(children.begin() + pos, childItem);

    if(m_model)
    {
        childItem->propagateModel(m_model);
        m_model->endInsertRows();
    }

}

void JKTreeModelItem::setParent(JKTreeModelItem *parent)
{
    this->m_parent = parent;
}

int JKTreeModelItem::columnCount() const
{
    return m_column_count;
}

void JKTreeModelItem::setColumnCount(int count)
{
    if(m_column_count == count)
        return;

    bool ins = count > m_column_count;
    if(m_model)
    {
        if(ins)
            m_model->beginInsertColumns(index(), m_column_count, count - 1);
        else
            m_model->beginRemoveColumns(index(), count, m_column_count - 1);
    }

    this->m_column_count = count;

    if(m_model)
    {
        if(ins)
            m_model->endInsertColumns();
        else
            m_model->endRemoveColumns();
    }
}

void JKTreeModelItem::propagateModel(JKTreeModel *model)
{
    m_model = model;
    for(auto &c: children)
        c->propagateModel(model);
}

QModelIndex JKTreeModelItem::index() const
{
    if(!m_model)
        return QModelIndex();

    return m_model->indexFromItem(this);
}

void JKTreeModelItem::update() const
{
    if(m_model)
    {
        QModelIndex i = index();
        m_model->dataChanged(i.sibling(i.row(), 0),
                             i.sibling(i.row(), m_column_count - 1));
    }
}

void JKTreeModelItem::updateRecursive() const
{
    if(m_model)
    {
        update();

        for(auto &c: children)
            c->updateRecursive();
    }
}

void JKTreeModelItem::update(int column) const
{
    if(m_model)
    {
        QModelIndex i = index();
        QModelIndex targetIndex = i.sibling(i.row(), column);
        m_model->dataChanged(targetIndex, targetIndex);
    }
}
