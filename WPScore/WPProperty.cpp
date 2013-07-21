#include "WPProperty.h"

WPProperty::WPProperty()
{
}

WPProperty::WPProperty(const WPInterval &I, const WPPropertyType &T, const std::string &S)
{
	Interval = I;
	Type = T;
	Arg = S;
}

WPProperty::~WPProperty()
{
}

WPInterval WPProperty::getInterval() const
{
	return Interval;
}

WPProperty::WPPropertyType WPProperty::getType() const
{
	return Type;
}

std::string WPProperty::getArg() const
{
	return Arg;
}