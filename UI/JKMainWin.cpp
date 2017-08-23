#include "stdafx.h"
#include "JKMainWin.h"

#include "BLL/JKProjectBLL.h"
#include "BLL/JKStockCodeBLL.h"

JKMainWin::JKMainWin(JKProjectBLL* _projectBLL, QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	refProject = _projectBLL;

	connect(ui.actNewStockCode, SIGNAL(triggered()), this, SLOT(NewStockCode()));

}

JKMainWin::~JKMainWin()
{

}

void JKMainWin::NewStockCode()
{

	JKRef_Ptr<JKStockCodeBLL> refStockCode = JKStockCodeBLL::NewStockCodeBLL();
	
}