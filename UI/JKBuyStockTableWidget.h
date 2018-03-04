#pragma once

#include "JKVirtualTreeAdapter.h"
#include <QTableView>


class JKStockCodeBLL;

class JKBuyStockTableViewer : public QTableView
{
	Q_OBJECT

public:
	JKBuyStockTableViewer(QWidget* parent = nullptr);
	virtual ~JKBuyStockTableViewer();

	virtual void setModel(QAbstractItemModel *model) override;

	/*void getSelectedStockTradeIds(std::vector<JKString> &_vecStockTradeIDs);
	void getSelectedStockTrade(std::vector<StockCodeTradeBLLConstRefPtr> &_vecStockTrade);*/


private:
	/*void initHeader();
	void initUI();*/

};


class JKBuyStockTableAdapter : public JKVirtualModelAdapter
{
public:
	JKBuyStockTableAdapter(BaseObjectConstRefPtr _stockCodeBll);
	~JKBuyStockTableAdapter();

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





