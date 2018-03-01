#pragma once

#include "bll/JKBaseBLL.h"
#include "Model/JKProjectModel.h"

class JKStockCodeBLL;

class JKProjectBLL : public JKBaseBLL<JKProjectModel>
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
	~JKProjectBLL();

	virtual void upgradeDataVersion(int dataVersion) override;
	
	static JKRef_Ptr<JKProjectBLL> newProject(const JKString &path);
	static JKRef_Ptr<JKProjectBLL> openProject(const JKString &path);

	JKRef_Ptr<JKStockCodeBLL> newStockCode();
	JKRef_Ptr<JKStockCodeBLL> getCurStockCode();
	vector<JKRef_Ptr<JKStockCodeBLL>> getAllStockCode();

	void setProjectSetting(JKRef_Ptr<JKProjectSettingBLL>);
	JKRef_Ptr<JKProjectSettingBLL> getProjectSetting();

	void deleteStockCode(JKRef_Ptr<JKStockCodeBLL> _refStockCode);
	//获取印花税
	float getStampTax();
	void setStampTax(float);
	//获取过户税
	float getTransfer();
	void setTransfer(float);
	//获取佣金
	float getCommission();
	void setCommission(float);

	void setCurStockCode(JKRef_Ptr<JKStockCodeBLL> stockCode);

private:
	JKRef_Ptr<JKStockCodeBLL> refCurStockCode;
};

using JKProjectBLLPtr = JKRef_Ptr<JKProjectBLL>;
