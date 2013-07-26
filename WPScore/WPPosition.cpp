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

bool WPPosition::operator < (const WPPosition &F) const
{
	return Value < F.Value;
}

bool WPPosition::operator <= (const WPPosition &F) const
{
	return Value <= F.Value;
}

bool WPPosition::operator > (const WPPosition &F) const
{
	return Value > F.Value;
}

bool WPPosition::operator >= (const WPPosition &F) const
{
	return Value >= F.Value;
}

WPPosition WPPosition::operator = (const Fraction &F)
{
	Value = F;
	return *this;
}

WPPosition WPPosition::operator + (const Fraction &F) const
{
	WPPosition Result(Value + F);
	return Result;
}

Fraction Fraction::operator += (const Fraction &F)
{
	*this = *this + F;
	return *this;
}
