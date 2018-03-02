#include "stdafx.h"
#include "JKProjectBLL.h"
#include "Model/JKProjectModel.h"
#include "JKStockCodeBLL.h"
#include "JKStockCodeTradeBLL.h"
#include "Model/JKModelFactory.h"
#include "JKBLLContext.h"
#include "JKProjectSettingBLL.h"
#include "JKBLLContainer.h"


JKRef_Ptr<JKProjectBLL> JKProjectBLL::newProject(const JKString &fileName)
{
	if (JKSingleton<JKDatabase>::GetInstance().newDatabase(fileName))
	{
		JKRef_Ptr<JKProjectBLL> refProject = NewBLL(JKProjectBLL, JKProjectModel);

		JKRef_Ptr<JKProjectSettingBLL> _refProjectSetting = NewBLL(JKProjectSettingBLL, JKProjectSettingModel);
		refProject->setProjectSetting(_refProjectSetting);

		return refProject;
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
		JKRef_Ptr<JKProjectBLL> refProject;

		std::list<JKRef_Ptr<JKProjectBLL>> listBll = LoadALLBLL(JKProjectBLL, JKProjectModel);
		if (listBll.size() > 0)
		{
			refProject = *listBll.begin();
			bean_ptr<JKProjectModel> ptrModel = refProject->getModel();
			if (ptrModel->vecStockCode.size() > 0)
			{
				JKRef_Ptr<JKStockCodeBLL> refCurStockCode = LoadBLL(JKStockCodeBLL, JKStockCodeModel, ptrModel->vecStockCode[0].get_id());
				refProject->setCurStockCode(refCurStockCode);
			}
		}
		else
		{
			refProject = NewBLL(JKProjectBLL, JKProjectModel);
		}
		BLLContext.setProject(refProject);

		try
		{
			refProject->upgradeDataVersion(JKSingleton<JKDatabase>::GetInstance().getDbDataVersion());
			JKSingleton<JKDatabase>::GetInstance().updateDbDataVersion();
		}
		catch (std::exception &e)
		{
			throw e;
		}

		return refProject;
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
	JKRef_Ptr<JKStockCodeBLL> _refStockCode = NewBLL(JKStockCodeBLL, JKStockCodeModel);
	_refStockCode->setParentID(parentID);

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
		JKRef_Ptr<JKStockCodeBLL> _refStockCodeTradeBLL = LoadBLL(JKStockCodeBLL, JKStockCodeModel, var.get_id());
		vecTrades.push_back(_refStockCodeTradeBLL);
	}

	return vecTrades;
}

JKRef_Ptr<JKProjectSettingBLL> JKProjectBLL::getProjectSetting()
{
	JKRef_Ptr<JKProjectSettingBLL> _refProjectSetting;
	if (ptrModel->projectSetting.get_id() == -1)
	{
		_refProjectSetting = NewBLL(JKProjectSettingBLL, JKProjectSettingModel);
		ptrModel->projectSetting = _refProjectSetting->getModel();
	}
	else
	{
		_refProjectSetting = LoadBLL(JKProjectSettingBLL, JKProjectSettingModel, ptrModel->projectSetting.get_id());
	}

	return _refProjectSetting;
}

void JKProjectBLL::deleteStockCode(JKRef_Ptr<JKStockCodeBLL> _refStockCode)
{
	if (refCurStockCode == _refStockCode)
		refCurStockCode = nullptr;

	ptrModel->deleteStockCode(_refStockCode->getModel());
	DestroyBLL(JKStockCodeBLL, JKStockCodeModel, refCurStockCode);
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

void JKProjectBLL::save()
{
	SaveBean(JKProjectSettingBLL, JKProjectSettingModel, ptrModel->projectSetting);
	for (int i = 0; i < ptrModel->vecStockCode.size(); ++i)
	{
		SaveBean(JKStockCodeBLL, JKStockCodeModel, ptrModel->vecStockCode[i]);
	}
}

void JKProjectBLL::destroy()
{
	refCurStockCode = nullptr;
	DestroyBean(JKProjectSettingBLL, JKProjectSettingModel, ptrModel->projectSetting);
	for (int i = 0; i < ptrModel->vecStockCode.size(); ++i)
	{
		DestroyBean(JKStockCodeBLL, JKStockCodeModel, ptrModel->vecStockCode[i]);
	}
}