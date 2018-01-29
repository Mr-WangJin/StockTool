#include "stdafx.h"
#include "JKUiContext.h"
#include "QApplication"
#include "JKMainWin.h"

JKString JKUiContext::getRecentFilePath()
{
	return (QApplication::applicationDirPath() + "\\recentOpenProject.json").toStdString();
}

void JKUiContext::setMainWin(JKMainWin* _mainWin)
{
	mainWin = _mainWin;
}
JKMainWin* JKUiContext::getMainWin()
{
	return mainWin;
}

JKRef_Ptr<JKProjectBLL> JKUiContext::getProjectBLL()
{
	if (mainWin)
	{
		return mainWin->getProjectBLL();
	}
	return nullptr;
}

JKUiContext::JKUiContext()
	: mainWin(nullptr)
{
}

JKUiContext::~JKUiContext()
{
}
