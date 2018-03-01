#pragma once

#include "bll/JKBaseBLL.h"
#include "Model/JKProjectSettingModel.h"


class JKProjectSettingBLL : public JKBaseBLL<JKProjectSettingModel>
{
	friend class JKProjectBLL;

	JK_BLL_INIT(JKProjectSetting);

public:
	bool getIsStartCrawl();
	void setIsStartCrawl(bool);

	bool getIsAlert();
	void setIsAlert(bool);

	double getUpAlertPercent();
	void setUpAlertPercent(double _upAlertPercent);

	double getDownAlertPercent();
	void setDownAlertPercent(double _downAlertPercent);

	int getTableShowType();
	void setTableShowType(int type);
};

