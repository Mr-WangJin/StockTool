#include "stdafx.h"
#include "JKUiEventDispatcher.h"

JKUiEventDispatcher::JKUiEventDispatcher()
{

}
JKUiEventDispatcher::~JKUiEventDispatcher()
{

}

bool JKUiEventDispatcher::registerUIEvent(JKUiEventType type, JKUiEventHandler* obj)
{
	assert(obj);

	bool bHas = false;
	std::pair <RegisterTypeIter, RegisterTypeIter> ret;
	ret = mMapRegister.equal_range(type);
	for (RegisterTypeIter it = ret.first; it != ret.second; ++it)
	{
		if (it->second == obj)
			bHas = true;
	}
	if (bHas == false)
	{
		mMapRegister.insert(std::make_pair(type, obj));
	}
	return true;
}

bool JKUiEventDispatcher::unregisterUIEvent(JKUiEventType type, QObject* obj)
{
	return false;
}

void JKUiEventDispatcher::customEvent(QEvent *event)
{
	if (event->type() == JKCustomUiEventCode)
	{
		JKCustomUIEvent* pEvent = dynamic_cast<JKCustomUIEvent*>(event);
		JKUiEventType uiType = pEvent->getUiEventType();

		std::pair <RegisterTypeIter, RegisterTypeIter> ret;
		ret = mMapRegister.equal_range(uiType);
		for (RegisterTypeIter it = ret.first; it != ret.second; ++it)
		{
			it->second->HandleEvent(pEvent);
		}
	}
}

