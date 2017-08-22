#include "stdafx.h"
#include "JKMainWin.h"

#include "BLL/JKProjectBLL.h"

JKMainWin::JKMainWin(JKProjectBLL* _projectBLL, QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	refProjectBLL = _projectBLL;

}

JKMainWin::~JKMainWin()
{

}
