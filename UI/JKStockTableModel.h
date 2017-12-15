#pragma once

#include <QAbstractTableModel>
#include <vector>

class JKProjectBLL;
class JKStockCodeTradeBLL;

enum TableShowType {
	Show_Buy_Only = 1,
	Show_Sell_Only = 1<<1,
	Show_All = Show_Buy_Only | Show_Sell_Only
};


class JKStockTableModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	JKStockTableModel(JKRef_Ptr<JKProjectBLL> refProject, QObject* parent = nullptr);
	virtual ~JKStockTableModel();

	void setProject(JKRef_Ptr<JKProjectBLL> refProject);
	void setShowType(TableShowType _showType);
	JKRef_Ptr<JKStockCodeTradeBLL> getStockTradeByRow(int row);

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

	Qt::ItemFlags flags(const QModelIndex &index) const override;

	virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;

private:
	JKRef_Ptr<JKProjectBLL> refProject;
	TableShowType showType;

	int colCount = 9;


	vector<JKRef_Ptr<JKStockCodeTradeBLL>> vecRefStockCodeTradeBLL;
};