#pragma once


class JKUiContext
{
	friend class JKSingleton<JKUiContext>;
public:
	JKString getRecentFilePath();

private:
	JKUiContext();
	~JKUiContext();

private:


};


