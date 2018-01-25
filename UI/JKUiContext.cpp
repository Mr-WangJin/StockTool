#include "stdafx.h"
#include "JKUiContext.h"
#include "QApplication"

JKString JKUiContext::getRecentFilePath()
{
	return (QApplication::applicationDirPath() + "\\recentOpenProject.json").toStdString();
}

JKUiContext::JKUiContext()
{
}

JKUiContext::~JKUiContext()
{
}
