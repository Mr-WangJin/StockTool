#ifndef TREEMODELSTANDARDITEM_H
#define TREEMODELSTANDARDITEM_H

#include <QMap>

#include "JKTreeModelCustomItem.h"

template<class Value>
class JKTreeModelRootItem : public JKTreeModelCustomItem<Value>
{
public:
	explicit                JKTreeModelRootItem(const Value &value);
    explicit                JKTreeModelRootItem(const QString &text, const QPixmap &pixmap = QPixmap());
	virtual					~JKTreeModelRootItem() {}

    void                    setPixmap(const QPixmap &pixmap, int column = 0);
    void                    setToolTip(const QString &toolTip, int column = 0);
    QVariant                data(int column, int role) const;
    bool                    setData(int column, const QVariant &data, int role);

private:
    typedef QMap<int, QVariant> CellData;

    QMap<int, CellData>     m_data;
    int                     m_columnCount;
};


template<class Value>
inline JKTreeModelRootItem<Value>::JKTreeModelRootItem(const Value & value) 
	: JKTreeModelCustomItem(value)
{}


template<class Value>
JKTreeModelRootItem<Value>::JKTreeModelRootItem(const QString &text, const QPixmap &pixmap) 
	: JKTreeModelItem(1)
{
	setData(0, text, Qt::DisplayRole);
	setPixmap(pixmap);
}


template<class Value>
void JKTreeModelRootItem<Value>::setPixmap(const QPixmap &pixmap, int column)
{
	if (!pixmap.isNull())
		setData(column, QVariant::fromValue(pixmap), Qt::DecorationRole);
	else
		setData(column, QVariant(), Qt::DecorationRole);
}

template<class Value>
void JKTreeModelRootItem<Value>::setToolTip(const QString &toolTip, int column)
{
	if (!toolTip.isEmpty())
		setData(column, toolTip, Qt::ToolTipRole);
	else
		setData(column, QVariant(), Qt::ToolTipRole);
}

template<class Value>
bool JKTreeModelRootItem<Value>::setData(int column, const QVariant &value, int role)
{
	if (column >= columnCount())
		return false;

	if (!value.isNull())
		m_data[column].insert(role, value);
	else
		m_data[column].remove(role);
	return true;
}

template<class Value>
QVariant JKTreeModelRootItem<Value>::data(int column, int role) const
{
	if (m_data.contains(column) &&
		m_data[column].contains(role))
		return m_data[column].value(role);

	return QVariant();
}


#endif // TREEMODELSTANDARDITEM_H
