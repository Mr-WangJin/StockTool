#include "stdafx.h"
#include "UI/JKMainWin.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	JKMainWin w;
	w.show();
	return a.exec();
}
