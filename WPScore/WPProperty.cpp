#include "WPProperty.h"

WPProperty::WPProperty()
{
}

WPProperty::WPProperty(const WPInterval &I, const std::string &S)
{
	Interval = I;
	Arg = S;
}

WPProperty::~WPProperty()
{
}

WPInterval WPProperty::getInterval() const
{
	return Interval;
}

std::string WPProperty::getArg() const
{
	return Arg;
}

void WPProperty::lengthen(const Fraction &F)
{
	Interval.lengthen(F);
}

void WPProperty::shiftRight(const Fraction &F)
{
	Interval.shiftRight(F);
}
