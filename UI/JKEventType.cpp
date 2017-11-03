#include "stdafx.h"
#include "JKEventType.h"



JKCustomUIEvent::JKCustomUIEvent(JKUiEventType type)
	: QEvent((QEvent::Type)(JKCustomUiEventCode))
	, uiEventType(type)
{
}

JKCustomUIEvent::~JKCustomUIEvent()
{
}