#include "stdafx.h"
#include "UI/JKMainWin.h"
#include <QtWidgets/QApplication>
#include "Utils/MiniDump/JKMiniDump.h"

int main(int argc, char *argv[])
{
	JKMiniDump dump;

	QApplication a(argc, argv);

	try {
		JKMainWin w;
		w.show();
		w.showAbout();

		return a.exec();
	}
	catch (std::exception & e)
	{
		QMessageBox::information(nullptr, QStringLiteral("��ʾ"), e.what());

	}
	catch (...)
	{
		QMessageBox::information(nullptr, QStringLiteral("��ʾ"), QStringLiteral("δ֪�쳣"));
	}
}
