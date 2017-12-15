#pragma once

#include <QTableView>

class JKStockTableViewer : public QTableView
{
	Q_OBJECT

public:
	JKStockTableViewer(QWidget* parent = nullptr);
	virtual ~JKStockTableViewer();

	virtual void setModel(QAbstractItemModel *model) override;

	void getSelectedStockCode(std::vector<JKString> &_vecStockTradeIDs);


private:
	void initHeader();
	void initUI();

};