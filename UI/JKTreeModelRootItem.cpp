#include "stdafx.h"
#include "JKTreeModelRootItem.h"

JKTreeModelRootItem::JKTreeModelRootItem(int columns):
    JKTreeModelItem(columns) {}

JKTreeModelRootItem::JKTreeModelRootItem(const QString &text, const QPixmap &pixmap):
    JKTreeModelItem(1)
{
    setData(0, text, Qt::DisplayRole);
    setPixmap(pixmap);
}

void JKTreeModelRootItem::setPixmap(const QPixmap &pixmap, int column)
{
    if(!pixmap.isNull())
        setData(column, QVariant::fromValue(pixmap), Qt::DecorationRole);
    else
        setData(column, QVariant(), Qt::DecorationRole);
}

void JKTreeModelRootItem::setToolTip(const QString &toolTip, int column)
{
    if(!toolTip.isEmpty())
        setData(column, toolTip, Qt::ToolTipRole);
    else
        setData(column, QVariant(), Qt::ToolTipRole);
}

bool JKTreeModelRootItem::setData(int column, const QVariant &value, int role)
{
    if(column >= columnCount())
        return false;

    if(!value.isNull())
        m_data[column].insert(role, value);
    else
        m_data[column].remove(role);
    return true;
}

QVariant JKTreeModelRootItem::data(int column, int role) const
{
    if(m_data.contains(column) &&
       m_data[column].contains(role))
        return m_data[column].value(role);

    return QVariant();
}
