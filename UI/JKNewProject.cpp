#include "stdafx.h"
#include "JKNewProject.h"
#include "BLL\JKProjectBLL.h"
#include "BLL\JKStockCodeBLL.h"
#include "BLL\JKProjectSettingBLL.h"

JKNewProject::JKNewProject(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint | Qt::WindowMinMaxButtonsHint);

	connect(ui.pBtnOK, SIGNAL(clicked()), this, SLOT(onOkClick()));
	connect(ui.pBtnCancel, SIGNAL(clicked()), this, SLOT(onCancelClick()));
}

JKNewProject::~JKNewProject()
{

}

double JKNewProject::getStampTax()
{
	return ui.dSpBxStampTax->value() / 1000.0f;
}
double JKNewProject::getTransfer()
{
	return ui.dSpBxTransfer->value() / 1000.0f;
}
double JKNewProject::getCommission()
{
	return ui.dSpBxCommission->value() / 1000.0f;
}

double JKNewProject::getUpAlertPercent()
{
	return ui.dSpBxUpAlertPercent->value() / 100.0f;
}

double JKNewProject::getDownAlertPercent()
{
	return ui.dSpBxDownAlertPercent->value() / 100.0f;
}

void JKNewProject::setProject(JKRef_Ptr<JKProjectBLL> _refProject)
{
	assert(_refProject.valid());
	ui.dSpBxStampTax->setValue(_refProject->getStampTax() * 1000.0f);
	ui.dSpBxCommission->setValue(_refProject->getCommission() * 1000.0f);
	ui.dSpBxTransfer->setValue(_refProject->getTransfer()* 1000.0f);
	
	JKRef_Ptr<JKProjectSettingBLL> _refProjectSetting = _refProject->getProjectSetting();
	ui.dSpBxUpAlertPercent->setValue(_refProjectSetting->getUpAlertPercent()*100.0f);
	ui.dSpBxDownAlertPercent->setValue(_refProjectSetting->getDownAlertPercent()*100.0f);
}

void JKNewProject::onOkClick()
{
	this->accept();
}

void JKNewProject::onCancelClick()
{
	this->reject();
}
