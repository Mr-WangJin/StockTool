#include "stdafx.h"
#include "UI/JKMainWin.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);


	try {

		JKMainWin w;
		w.show();

		return a.exec();

	}
	catch (std::exception & e)
	{
		QMessageBox::information(nullptr, QStringLiteral("提示"), e.what());

	}
	catch (...)
	{
		QMessageBox::information(nullptr, QStringLiteral("提示"), QStringLiteral("未知异常"));
	}
}
