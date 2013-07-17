#include "WPInterval.h"

WPInterval::WPInterval()
{
}

WPInterval::WPInterval(const WPPosition &X, const WPPosition &Y)
{
	Begin = X;
	End = Y;
}

WPInterval::~WPInterval()
{
}

WPPosition WPInterval::begin() const
{
	return Begin;
}
WPPosition WPInterval::end() const
{
	return End;
}
