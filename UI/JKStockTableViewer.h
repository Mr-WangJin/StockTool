#pragma once

#include <QTableView>

class JKStockCodeTradeBLL;

class JKStockTableViewer : public QTableView
{
	Q_OBJECT

public:
	JKStockTableViewer(QWidget* parent = nullptr);
	virtual ~JKStockTableViewer();

	void resizeColumnsWidth();
	virtual void setModel(QAbstractItemModel *model) override;

	void getSelectedStockTradeIds(std::vector<JKString> &_vecStockTradeIDs);
	void getSelectedStockTrade(std::vector<JKRef_Ptr<JKStockCodeTradeBLL>> &_vecStockTrade);


private:
	void initHeader();
	void initUI();

};