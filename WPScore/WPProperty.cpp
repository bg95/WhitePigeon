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

bool WPProperty::operator < (const WPProperty &P) const
{
	if (Interval.begin() < P.getInterval().begin())
		return 1;
	if (Interval.begin() == P.getInterval().begin() && Interval.end() < P.getInterval().end())
		return 1;
    if (Interval.begin() == P.getInterval().begin() && Interval.end() == P.getInterval().end() && Arg < P.getArg())
		return 1;
	return 0;
}

std::vector <WPProperty> filterPrefix(const std::vector <WPProperty> &VP, const std::string &Pfx)
{
	std::vector <WPProperty> Result;
	for (std::vector <WPProperty> :: const_iterator it = VP.begin(); it != VP.end(); ++ it)
		if (it->getArg().find(Pfx) == 0)
			Result.push_back(*it);
	return Result;
}
