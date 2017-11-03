#pragma once

#include <QEvent>

#define JKCustomUiEventCode QEvent::User + 2000


enum class JKUiEventType
{
	NoEventType = 1,
	CrawlStockPriceFinish,		
};


class JKCustomUIEvent : public QEvent
{
public:
	JKCustomUIEvent(JKUiEventType type = JKUiEventType::NoEventType);
	~JKCustomUIEvent();

	JKUiEventType getUiEventType() const { return uiEventType; };
	const std::vector<JKUInt64> & getVecIdArgv() const{ return vecIdArgv; };
	const std::vector<JKString> & getVecStrArgv() const{ return vecStrArgv; };

private:
	JKUiEventType				uiEventType;
	std::vector<JKUInt64>		vecIdArgv;
	std::vector<JKString>		vecStrArgv;
};

class JKUiEventHandler
{
public:
	JKUiEventHandler() {};
	virtual ~JKUiEventHandler() {};


	virtual void HandleEvent(const JKCustomUIEvent* event) {};

};
