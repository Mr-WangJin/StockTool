#ifndef TREEMODELITEM_H
#define TREEMODELITEM_H

#include <QList>
#include <QVariant>
#include <QPixmap>
#include <QSharedPointer>
#include <QMap>

class JKTreeModel;
class JKTreeModelItem;
typedef QSharedPointer<JKTreeModelItem> TreeModelItemPtr;

class JKTreeModelItem
{
public:
    explicit                JKTreeModelItem(int columns);
    virtual                 ~JKTreeModelItem() {}

    virtual bool            setData(int column, const QVariant &data, int role){ Q_UNUSED(column); Q_UNUSED(data); Q_UNUSED(role); return false; }
    virtual QVariant        data(int column, int role) const {Q_UNUSED(column); Q_UNUSED(role); return QVariant(); }
    virtual Qt::ItemFlags   flags(int column) const;

    JKTreeModelItem         *child(int number);
    const JKTreeModelItem   *child(int number) const;
    int                     childCount() const;
    int                     childPosition(const JKTreeModelItem *item) const;
    JKTreeModelItem         *parent();
    const JKTreeModelItem   *parent() const;
    bool                    removeChildren(int position, int count);
    void                    removeChildren();
    void                    moveHere(TreeModelItemPtr item, int pos);
    TreeModelItemPtr        takeChild(int number);
    void                    detachChild(TreeModelItemPtr ptr);

    void                    prependChild(JKTreeModelItem *childItem);
    void                    appendChild(JKTreeModelItem *childItem);
    void                    insertChild(int pos, JKTreeModelItem *childItem);
    void                    insertChild(int pos, TreeModelItemPtr childItem);
    void                    setParent(JKTreeModelItem *parent);

    int                     columnCount() const;
    void                    setColumnCount(int count);

    void                    propagateModel(JKTreeModel *model);
    QModelIndex             index() const;
    void                    update() const;
    void                    updateRecursive() const;
    void                    update(int column) const;

	void					sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

private:
    int                     m_column_count;
    JKTreeModelItem         *m_parent = 0;
    QList<TreeModelItemPtr> children;
    JKTreeModel             *m_model = 0;
};

#endif // TREEMODELITEM_H
