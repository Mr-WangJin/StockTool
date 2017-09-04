#include "stdafx.h"
#include "UI/JKMainWin.h"
#include <QtWidgets/QApplication>
#include "bll/JKProjectBLL.h"


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	JKRef_Ptr<JKProjectBLL> projectBLL = new JKProjectBLL(JKProjectBLL::ProjectInitStatus::DEFAULT_FirstOrNew);

	JKMainWin w(projectBLL.get());
	w.show();
	return a.exec();
}
