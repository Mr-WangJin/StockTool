#pragma once

#include "JKVirtualTreeAdapter.h"
#include <QTableView>


class JKStockCodeBLL;

class JKBuyStockTableAdapter : public JKVirtualModelAdapter
{
public:
	JKBuyStockTableAdapter(BaseObjectConstRefPtr _stockCodeBll);
	JKBuyStockTableAdapter(const JKBuyStockTableAdapter *);
	~JKBuyStockTableAdapter();

	void setRoot(BaseObjectConstRefPtr _root) { root = _root; };
	int getItemsCount(BaseObjectConstRefPtr parent) override;

	QVariant data(BaseObjectConstRefPtr item, int role, const QModelIndex &index) override;
	QVariant headerData(int section, Qt::Orientation orientation, int role) override;

	BaseObjectPtr getItem(BaseObjectConstRefPtr parent, int index) override;
	BaseObjectPtr getItemParent(BaseObjectConstRefPtr item) override;

	virtual int getColumnCount() override;

	inline BaseObjectPtr getValue(BaseObjectConstRefPtr data);

private:
	BaseObjectPtr root;

	int tradeType = (int)TradeType::BUY;

};





