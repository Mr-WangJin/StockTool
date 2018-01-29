#pragma once


class JKMainWin;
class JKProjectBLL;

class JKUiContext
{
	friend class JKSingleton<JKUiContext>;
public:
	JKString getRecentFilePath();

	void setMainWin(JKMainWin* _mainWin);
	JKMainWin* getMainWin();

	JKRef_Ptr<JKProjectBLL> getProjectBLL();

private:
	JKUiContext();
	~JKUiContext();

private:
	JKMainWin* mainWin;

};


