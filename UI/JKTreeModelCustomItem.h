#ifndef TREEMODELCUSTOMITEM_H
#define TREEMODELCUSTOMITEM_H

#include "JKTreeModelItem.h"
#include "JKModelDataAdapter.h"

template<class Value>
class JKTreeModelCustomItem: public JKTreeModelItem,
        public JKModelDataAdapter<Value>
{
public:
    explicit                JKTreeModelCustomItem(const Value &value):
        JKTreeModelItem(0),
        value(value) {}

    void                    setValue(const Value &value)        {this->value = value; update();}
    const Value             &getValue() const                   {return value;}

    QVariant                data(int column, int role) const
    {
        return JKModelDataAdapter<Value>::data(value, column, role);
    }
    bool                    setData(int column, const QVariant &data, int role)
    {
        bool result = JKModelDataAdapter<Value>::setData(&value, column, data, role);
        update();
        return result;
    }
    Qt::ItemFlags           flags(int column) const
    {
        return this->getFlags(column);
    }

private:
    void columnManipulatorAdded(int column) override
    {
        setColumnCount(std::max(column + 1, columnCount()));
    }

    Value   value;
};

template<class Value>
class JKTreeModelCustomItem<Value *>: public JKTreeModelItem,
        public JKModelDataAdapter<Value>
{
public:
    explicit JKTreeModelCustomItem(Value *value):
        JKTreeModelItem(0),
        value(value) {}

    void                    setValue(Value *value)      {this->value = value; update();}
    const Value             *getValue() const           {return value;}

    QVariant                data(int column, int role) const
    {
        return JKModelDataAdapter<Value>::data(*value, column, role);
    }
    bool                    setData(int column, const QVariant &data, int role)
    {
        return JKModelDataAdapter<Value>::setData(value, column, data, role);
    }
    Qt::ItemFlags           flags(int column) const
    {
        return this->getFlags(column);
    }

private:
    void columnManipulatorAdded(int column) override
    {
        setColumnCount(std::max(column + 1, columnCount()));
    }

    Value *value;
};

#endif // TREEMODELCUSTOMITEM_H
