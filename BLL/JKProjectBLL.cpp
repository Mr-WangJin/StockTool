#include "stdafx.h"
#include "JKProjectBLL.h"
#include "Model/JKProjectModel.h"
#include "JKStockCodeBLL.h"
#include "JKStockCodeTradeBLL.h"
#include "Model/JKModelFactory.h"
#include "JKBLLContext.h"
#include "JKProjectSettingBLL.h"


JKProjectBLL::JKProjectBLL(ProjectInitStatus status)
{
	if (status == ProjectInitStatus::DEFAULT_FirstOrNew)
	{
		std::vector<sqlid_t> vecIds = SingleDB->getBeanIds<JKProjectModel>();
		if (vecIds.size() > 0)
		{
			ptrModel = SingleDB->loadBean<JKProjectModel>(vecIds[0]);
			if (ptrModel->vecStockCode.size() > 0)
				refCurStockCode = new JKStockCodeBLL(ptrModel->vecStockCode[0], refContext);
		}
		else
		{
			ptrModel = SingleDB->createBean<JKProjectModel>();
		}
	}

	JKBLLContext* context = new JKBLLContext();
	context->setProject(this);
	this->setContext(context);
}

JKRef_Ptr<JKProjectBLL> JKProjectBLL::newProject(const JKString &fileName)
{
	if (JKSingleton<JKDatabase>::GetInstance().newDatabase(fileName))
	{
		JKRef_Ptr<JKProjectBLL> refProjectBLL = new JKProjectBLL(JKProjectBLL::ProjectInitStatus::DEFAULT_FirstOrNew);
		JKRef_Ptr<JKProjectSettingBLL> _refProjectSetting = new JKProjectSettingBLL();
		refProjectBLL->setProjectSetting(_refProjectSetting);

		return refProjectBLL;
	}
	else
	{
		throw std::exception(QStringLiteral("创建工程失败!").toStdString().c_str());
	}
}

JKRef_Ptr<JKProjectBLL> JKProjectBLL::openProject(const JKString & path)
{
	if (JKSingleton<JKDatabase>::GetInstance().openDatabase(path))
	{
		JKRef_Ptr<JKProjectBLL> refProjectBLL = new JKProjectBLL(JKProjectBLL::ProjectInitStatus::DEFAULT_FirstOrNew);
		
		try
		{
			refProjectBLL->upgradeDataVersion(JKSingleton<JKDatabase>::GetInstance().getDbDataVersion());
			JKSingleton<JKDatabase>::GetInstance().updateDbDataVersion();
		}
		catch (std::exception &e)
		{
			throw e;
		}

		return refProjectBLL;
	}
	return nullptr;
}

JKProjectBLL::~JKProjectBLL()
{
}

void JKProjectBLL::upgradeDataVersion(int dataVersion)
{
	vector<JKRef_Ptr<JKStockCodeBLL>> _vecStockCode = getAllStockCode();
	for (auto &var : _vecStockCode)
	{
		var->upgradeDataVersion(dataVersion);
	}

	if (dataVersion < 1)
	{

	}

}

JKRef_Ptr<JKStockCodeBLL> JKProjectBLL::newStockCode()
{
	JKRef_Ptr<JKStockCodeBLL> _refStockCode = new JKStockCodeBLL(refContext);
	ptrModel->addStockCode(_refStockCode->getModel());
	return _refStockCode;
}

void JKProjectBLL::setCurStockCode(JKRef_Ptr<JKStockCodeBLL> stockCode)
{
	refCurStockCode = stockCode;
}

JKRef_Ptr<JKStockCodeBLL> JKProjectBLL::getCurStockCode()
{
	return refCurStockCode;
}

void JKProjectBLL::setProjectSetting(JKRef_Ptr<JKProjectSettingBLL> _refProjectSetting)
{
	ptrModel->projectSetting = _refProjectSetting->getModel();
}

vector<JKRef_Ptr<JKStockCodeBLL>> JKProjectBLL::getAllStockCode()
{
	vector<JKRef_Ptr<JKStockCodeBLL>> vecTrades;

	for (auto &var : ptrModel->vecStockCode)
	{
		JKRef_Ptr<JKStockCodeBLL> _refStockCodeTradeBLL = new JKStockCodeBLL(var, refContext);
		vecTrades.push_back(_refStockCodeTradeBLL);
	}

	return vecTrades;
}

JKRef_Ptr<JKProjectSettingBLL> JKProjectBLL::getProjectSetting()
{
	if (ptrModel->projectSetting.get_id() == -1)
	{
		JKRef_Ptr<JKProjectSettingBLL> _refProjectSetting = new JKProjectSettingBLL();
		ptrModel->projectSetting = _refProjectSetting->getModel();
	}

	JKRef_Ptr<JKProjectSettingBLL> _refProjectSetting = new JKProjectSettingBLL(ptrModel->projectSetting, refContext);
	return _refProjectSetting;
}

void JKProjectBLL::deleteStockCode(JKRef_Ptr<JKStockCodeBLL> _refStockCode)
{
	if (refCurStockCode == _refStockCode)
		refCurStockCode = nullptr;

	ptrModel->deleteStockCode(_refStockCode->getModel());
	
}

float JKProjectBLL::getStampTax()
{
	return ptrModel->stampTax;
}
float JKProjectBLL::getTransfer()
{
	return ptrModel->transfer;
}
float JKProjectBLL::getCommission()
{
	return ptrModel->commission;
}
void JKProjectBLL::setStampTax(float stampTax)
{
	ptrModel->stampTax = stampTax;
}
void JKProjectBLL::setTransfer(float transfer)
{
	ptrModel->transfer = transfer;
}
void JKProjectBLL::setCommission(float commission)
{
	ptrModel->commission = commission;
}