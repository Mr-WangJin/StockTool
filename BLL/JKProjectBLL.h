#pragma once

#include "bll/JKBaseBLL.h"
#include "Model/JKProjectModel.h"
//#include "bll/JKStockCodeBLL.h"

class JKStockCodeBLL;

class JKProjectBLL : public JKBaseBLL
{
	JK_BLL_INIT(JKProject);

public:
	enum class ProjectInitStatus
	{
		DEFAULT_First,
		DEFAULT_New,
		DEFAULT_FirstOrNew
	};
public:

	JKProjectBLL(ProjectInitStatus status);
	~JKProjectBLL();
	
	static JKRef_Ptr<JKProjectBLL> newProject(const JKString &path);
	static JKRef_Ptr<JKProjectBLL> openProject(const JKString &path);

	void savePrject();


	JKRef_Ptr<JKStockCodeBLL> newStockCode();
	JKRef_Ptr<JKStockCodeBLL> getCurStockCode();
	vector<JKRef_Ptr<JKStockCodeBLL>> getAllStockCode();
	float getStampTax();
	float getTransfer();
	float getCommission();
	void setStampTax(float);
	void setTransfer(float);
	void setCommission(float);

	//获取成本价
	double getCostPrice(JKRef_Ptr<JKStockCodeTradeBLL>);
	//获取当前交易的买入税费
	double getBuyTaxes(JKRef_Ptr<JKStockCodeTradeBLL>);
	//获取当前交易的卖出税费
	double getSellTaxes(JKRef_Ptr<JKStockCodeTradeBLL>);
	//获取预计卖出税费
	double getPredictSellTaxes(JKRef_Ptr<JKStockCodeTradeBLL>, double sellPrice);
	//获取真实收益
	double getRealEarning(double latestPrice, JKRef_Ptr<JKStockCodeTradeBLL> refStockCodeTrade);




	void setCurStockCode(JKRef_Ptr<JKStockCodeBLL> stockCode);


private:
	JKRef_Ptr<JKStockCodeBLL> refCurStockCode;


};

