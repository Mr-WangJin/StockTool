#ifndef TREEMODELSTANDARDITEM_H
#define TREEMODELSTANDARDITEM_H

#include <QMap>

#include "JKTreeModelItem.h"

class JKTreeModelRootItem: public JKTreeModelItem
{
public:
    explicit                JKTreeModelRootItem(int columns);
    explicit                JKTreeModelRootItem(const QString &text, const QPixmap &pixmap = QPixmap());

    void                    setPixmap(const QPixmap &pixmap, int column = 0);
    void                    setToolTip(const QString &toolTip, int column = 0);
    QVariant                data(int column, int role) const;
    bool                    setData(int column, const QVariant &data, int role);

private:
    typedef QMap<int, QVariant> CellData;

    QMap<int, CellData>     m_data;
    int                     m_columnCount;
};

#endif // TREEMODELSTANDARDITEM_H
