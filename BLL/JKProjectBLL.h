#pragma once

#include "bll/JKBaseBLL.h"
#include "Model/JKProjectModel.h"

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

	virtual void upgradeDataVersion(int dataVersion) override;
	
	static JKRef_Ptr<JKProjectBLL> newProject(const JKString &path);
	static JKRef_Ptr<JKProjectBLL> openProject(const JKString &path);

	JKRef_Ptr<JKStockCodeBLL> newStockCode();
	JKRef_Ptr<JKStockCodeBLL> getCurStockCode();
	vector<JKRef_Ptr<JKStockCodeBLL>> getAllStockCode();
	//��ȡӡ��˰
	float getStampTax();
	void setStampTax(float);
	//��ȡ����˰
	float getTransfer();
	void setTransfer(float);
	//��ȡӶ��
	float getCommission();
	void setCommission(float);

	void setCurStockCode(JKRef_Ptr<JKStockCodeBLL> stockCode);

private:
	JKRef_Ptr<JKStockCodeBLL> refCurStockCode;
};

