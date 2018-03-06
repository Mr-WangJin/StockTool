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
	
	static ProjectBLLPtr newProject(const JKString &path);
	static ProjectBLLPtr openProject(const JKString &path);

	void saveProject();
	StockCodeBLLPtr newStockCode();
	StockCodeBLLPtr getCurStockCode();
	vector<JKRef_Ptr<JKStockCodeBLL>> getAllStockCode();

	void setProjectSetting(JKRef_Ptr<JKProjectSettingBLL>);
	ProjectSettingBLLPtr getProjectSetting();

	void deleteStockCode(StockCodeBLLConstRefPtr _refStockCode);
	//��ȡӡ��˰
	float getStampTax();
	void setStampTax(float);
	//��ȡ����˰
	float getTransfer();
	void setTransfer(float);
	//��ȡӶ��
	float getCommission();
	void setCommission(float);

	void setCurStockCode(StockCodeBLLConstRefPtr stockCode);

protected:
	virtual void save() override;
	virtual void destroy() override;


private:
	JKRef_Ptr<JKStockCodeBLL> refCurStockCode;
};

using JKProjectBLLPtr = JKRef_Ptr<JKProjectBLL>;
