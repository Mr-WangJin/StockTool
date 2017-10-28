#include "stdafx.h"
#include "JKNewProject.h"

JKNewProject::JKNewProject(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	
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

void JKNewProject::onOkClick()
{
	this->accept();
}

void JKNewProject::onCancelClick()
{
	this->reject();
}
