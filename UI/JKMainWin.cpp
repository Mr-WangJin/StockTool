#include "stdafx.h"
#include "JKMainWin.h"

#include "BLL/JKProjectBLL.h"
#include "JKFramework/SmartPtr/JKRef_Ptr.h"

JKMainWin::JKMainWin(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

JKMainWin::~JKMainWin()
{

}
