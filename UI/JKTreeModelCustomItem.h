#pragma once

#include "JKTreeModelItem.h"
#include "JKModelDataAdapter.h"

template<class Value>
class JKTreeModelCustomItem 
	: public JKTreeModelItem
	, public JKModelDataAdapter<Value>
{
public:
    explicit JKTreeModelCustomItem(const Value &value)
		: JKTreeModelItem(0),value(value) {}

    void					setValue(const Value &value) {this->value = value; update();}
	const Value				&getValue() const { return value; }
	virtual QVariant		data(int column, int role) const;
	bool					setData(int column, const QVariant &data, int role)
	{
		bool result = JKModelDataAdapter<Value>::setData(&value, column, data, role);
		update();
		return result;
	}
	Qt::ItemFlags			flags(int column) const { return this->getFlags(column); };

	virtual int			    childCount() const;


	template<class Function>
	void					addChildCountGetter(const Function &function);

	template<class Function>
	void					addInitChildFunc(const Function &function);

private:
	void					columnManipulatorAdded(int column) override { setColumnCount(std::max(column + 1, columnCount())); };


	std::function<size_t(Value)> childCountGetter;
	std::function<void(Value, JKTreeModelItem*)> initChildFunc;

    Value				value;
	bool				isLoadChild = false;

};



//////////////////////////////////////////////////////////////////////////

template<class Value>
class JKTreeModelCustomItem<Value *>
	: public JKTreeModelItem, public JKModelDataAdapter<Value>
{
public:
	explicit JKTreeModelCustomItem(Value *value)
		: JKTreeModelItem(0), value(value) {};

	void                    setValue(Value *value) { this->value = value; update(); };
	const Value             *getValue() const { return value; };
	QVariant                data(int column, int role) const { return JKModelDataAdapter<Value>::data(*value, column, role); };
	bool                    setData(int column, const QVariant &data, int role) 
	{
		return JKModelDataAdapter<Value>::setData(value, column, data, role); 
	};
	Qt::ItemFlags           flags(int column) const { return this->getFlags(column); };

private:
	void columnManipulatorAdded(int column) override { setColumnCount(std::max(column + 1, columnCount())); };

    Value *value;
};



template<class Value>
inline int JKTreeModelCustomItem<Value>::childCount() const
{
	if (!isLoadChild)
		return childCountGetter(value);
	
	return JKTreeModelItem::childCount();
}

template<class Value>
inline QVariant JKTreeModelCustomItem<Value>::data(int column, int role) const
{
	return JKModelDataAdapter<Value>::data(value, column, role); 
}



template<class Value>
template<class Function>
inline void JKTreeModelCustomItem<Value>::addChildCountGetter(const Function & function)
{
	childCountGetter = function;
}

template<class Value>
template<class Function>
inline void JKTreeModelCustomItem<Value>::addInitChildFunc(const Function & function)
{
	initChildFunc = function;
}
