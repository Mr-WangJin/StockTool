#include "stdafx.h"
#include "JKAbout.h"
#include "Model/JKDatabase.h"

JKAbout::JKAbout(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	ui.lblVersion->setText(QString("%3 0.%1.%2")
		.arg(JKDatabase::databaseVersion).arg(JKDatabase::dataVersion).arg(QStringLiteral("��ǰ�汾�ţ�")));
}

JKAbout::~JKAbout()
{

}
