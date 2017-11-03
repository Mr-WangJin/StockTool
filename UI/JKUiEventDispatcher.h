#pragma once

#include <QObject>
#include "JKEventType.h"

class JKUiEventDispatcher : public QObject
{
	Q_OBJECT
public:
	JKUiEventDispatcher();
	~JKUiEventDispatcher();


	bool registerUIEvent(JKUiEventType type, JKUiEventHandler* obj);
	bool unregisterUIEvent(JKUiEventType type, QObject* obj);

protected:
	virtual void customEvent(QEvent *) override;

private:
	using RegisterType = std::multimap<JKUiEventType, JKUiEventHandler*>;
	using RegisterTypeIter = std::multimap<JKUiEventType, JKUiEventHandler*>::iterator;
	RegisterType mMapRegister;


};
