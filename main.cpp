#include "stdafx.h"
#include "UI/JKMainWin.h"
#include <QtWidgets/QApplication>
#include "bll/JKProjectBLL.h"
#include <QMessageBox>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	try {

		JKRef_Ptr<JKProjectBLL> projectBLL = new JKProjectBLL(JKProjectBLL::ProjectInitStatus::DEFAULT_FirstOrNew);

		JKMainWin w(projectBLL.get());
		w.show();

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
