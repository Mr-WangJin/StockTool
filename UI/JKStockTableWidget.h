#pragma once

#include "JKVirtualTreeAdapter.h"


class JKBuyStockTableAdapter : public JKVirtualModelAdapter
{
public:
	JKBuyStockTableAdapter(ProjectBLLConstRefPtr _projectBll, BaseObjectConstRefPtr _stockCodeBll);
	//JKBuyStockTableAdapter(const JKBuyStockTableAdapter *);
	~JKBuyStockTableAdapter();

	virtual QVariant data(BaseObjectConstRefPtr item, int role, const QModelIndex &index) override;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role) override;

	virtual bool setData(BaseObjectConstRefPtr item, const QVariant &value, int role = Qt::EditRole) override;
	virtual Qt::ItemFlags flags(const QModelIndex & index) const override;

	virtual int getItemsCount(BaseObjectConstRefPtr parent) override;
	virtual BaseObjectPtr getItem(BaseObjectConstRefPtr parent, int index) override;
	virtual BaseObjectPtr getItemParent(BaseObjectConstRefPtr item) override;

	virtual int getColumnCount() override;

	inline BaseObjectPtr getValue(BaseObjectConstRefPtr data);

private:
	int columnCount = 0;
	int tradeType = (int)TradeType::BUY | (int)TradeType::PART;

	std::map<int, QVariant> mapHeader;

};


class JKSellStockTableAdapter : public JKVirtualModelAdapter
{
public:
	JKSellStockTableAdapter(ProjectBLLConstRefPtr _projectBll, BaseObjectConstRefPtr _stockCodeBll);
	//JKBuyStockTableAdapter(const JKBuyStockTableAdapter *);
	~JKSellStockTableAdapter();

	QVariant data(BaseObjectConstRefPtr item, int role, const QModelIndex &index) override;
	QVariant headerData(int section, Qt::Orientation orientation, int role) override;

	int getItemsCount(BaseObjectConstRefPtr parent) override;
	BaseObjectPtr getItem(BaseObjectConstRefPtr parent, int index) override;
	BaseObjectPtr getItemParent(BaseObjectConstRefPtr item) override;

	virtual int getColumnCount() override;

	inline BaseObjectPtr getValue(BaseObjectConstRefPtr data);

private:
	int columnCount = 0;
	int tradeType = (int)TradeType::SELL ;

	std::map<int, QVariant> mapHeader;

};





