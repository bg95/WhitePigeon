#include "WPPosition.h"

WPPosition::WPPosition()
{
}

WPPosition::~WPPosition()
{
}

WPPosition::WPPosition(const Fraction &Q)
{
	Value = Q;
}

Fraction WPPosition::getValue() const
{
	return Value;
}
