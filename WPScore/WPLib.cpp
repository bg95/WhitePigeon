#include "WPLib.h"

Fraction::Fraction()
{
}

Fraction::~Fraction()
{
}

Fraction::Fraction(int A, int B)
{
	X = A;
	Y = B;
}

bool Fraction::operator == (const Fraction &F) const
{
	return (long long) X * F.Y == (long long) F.X * Y;
}

bool Fraction::operator < (const Fraction &F) const
{
	return (long long) X * F.Y < (long long) F.X * Y;
}

double Fraction::toDouble() const
{
	return (double) X / (double) Y;
}
