#include "stdafx.h"
#include "JKProjectBLL.h"
#include "Model/JKProjectModel.h"
#include "JKStockCodeBLL.h"
#include "JKStockCodeTradeBLL.h"
#include "Model/JKModelFactory.h"
#include "JKBLLContext.h"


JKProjectBLL::JKProjectBLL(ProjectInitStatus status)
{
	if (status == ProjectInitStatus::DEFAULT_FirstOrNew)
	{
		std::vector<sqlid_t> vecIds = SingleDB->getBeanIds<JKProjectModel>();
		if (vecIds.size() > 0)
		{
			refJKProjectModel = SingleDB->loadBean<JKProjectModel>(vecIds[0]);
			if (refJKProjectModel->vecStockCode.size() > 0)
				refCurStockCode = new JKStockCodeBLL(refJKProjectModel->vecStockCode[0], refContext);
		}
		else
		{
			refJKProjectModel = SingleDB->createBean<JKProjectModel>();
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
	refJKProjectModel->addStockCode(_refStockCode->getModel());
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

vector<JKRef_Ptr<JKStockCodeBLL>> JKProjectBLL::getAllStockCode()
{
	vector<JKRef_Ptr<JKStockCodeBLL>> vecTrades;

	for (auto &var : refJKProjectModel->vecStockCode)
	{
		JKRef_Ptr<JKStockCodeBLL> _refStockCodeTradeBLL = new JKStockCodeBLL(var, refContext);
		vecTrades.push_back(_refStockCodeTradeBLL);
	}

	return vecTrades;
}

void JKProjectBLL::deleteStockCode(JKRef_Ptr<JKStockCodeBLL> _refStockCode)
{
	refJKProjectModel->deleteStockCode(_refStockCode->getModel());
}

float JKProjectBLL::getStampTax()
{
	return refJKProjectModel->stampTax;
}
float JKProjectBLL::getTransfer()
{
	return refJKProjectModel->transfer;
}
float JKProjectBLL::getCommission()
{
	return refJKProjectModel->commission;
}
void JKProjectBLL::setStampTax(float stampTax)
{
	refJKProjectModel->stampTax = stampTax;
}
void JKProjectBLL::setTransfer(float transfer)
{
	refJKProjectModel->transfer = transfer;
}
void JKProjectBLL::setCommission(float commission)
{
	refJKProjectModel->commission = commission;
}