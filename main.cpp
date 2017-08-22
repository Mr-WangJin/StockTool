#include "stdafx.h"
#include "UI/JKMainWin.h"
#include <QtWidgets/QApplication>
#include "BLL/JKProjectBLL.h"


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	JKProjectBLL projectBLL;

	JKMainWin w(&projectBLL);
	w.show();
	return a.exec();
}
