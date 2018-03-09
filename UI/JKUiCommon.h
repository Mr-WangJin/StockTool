#pragma once

#include "JKUiContext.h"

#define UP_EARNING QColor(255, 0, 0, 230)
#define DOWN_EARNING QColor(0, 255, 0, 230)


#define PROPERTY_UI_GETTER(valueType, name) \
	valueType get##name() {return this->##name;}

#define PROPERTY_UI_SETTER(valueType, name) \
	void set##name(valueType value) { this->##name = value;}

#define PROPERTY_UI(valueType, name) \
	PROPERTY_UI_GETTER(valueType, name)\
	PROPERTY_UI_SETTER(valueType, name)


enum TableShowType {
	Show_Buy_Only = 1,
	Show_Sell_Only = 1 << 1,
	Show_All = Show_Buy_Only | Show_Sell_Only
};
