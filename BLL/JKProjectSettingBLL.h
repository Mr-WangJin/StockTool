#pragma once

#include "bll/JKBaseBLL.h"
#include "Model/JKProjectSettingModel.h"


class JKProjectSettingBLL : public JKBaseBLL
{
	friend class JKProjectBLL;

	JK_BLL_INIT(JKProjectSetting);

public:
	JKProjectSettingBLL();

	bool getIsStartCrawl();
	void setIsStartCrawl(bool);

	bool getIsAlert();
	void setIsAlert(bool);

	double getAlertPercent();
	void setAlertPercent(double _alertPercent);
};

