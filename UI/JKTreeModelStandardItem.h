#ifndef TREEMODELSTANDARDITEM_H
#define TREEMODELSTANDARDITEM_H

#include <QMap>

#include "JKTreeModelItem.h"

class JKTreeModelStandardItem: public JKTreeModelItem
{
public:
    explicit                JKTreeModelStandardItem(int columns);
    explicit                JKTreeModelStandardItem(const QString &text, const QPixmap &pixmap = QPixmap());

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
