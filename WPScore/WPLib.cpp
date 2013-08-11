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

bool Fraction::operator <= (const Fraction &F) const
{
	return (long long) X * F.Y <= (long long) F.X * Y;
}

bool Fraction::operator > (const Fraction &F) const
{
	return (long long) X * F.Y > (long long) F.X * Y;
}

bool Fraction::operator >= (const Fraction &F) const
{
	return (long long) X * F.Y >= (long long) F.X * Y;
}

Fraction Fraction::operator = (const Fraction &F)
{
	X = F.X;
	Y = F.Y;
	return *this;
}

Fraction Fraction::operator + (const Fraction &F) const
{
	Fraction Result(F.Y / std::__gcd(Y, F.Y) * X + Y / std::__gcd(Y, F.Y) * F.X, Y / std::__gcd(Y, F.Y) * F.Y);
	int d = std::__gcd(Result.X, Result.Y);
	Result.X /= d;
	Result.Y /= d;
	return Result;
}

Fraction Fraction::operator += (const Fraction &F)
{
	*this = *this + F;
	return *this;
}

Fraction Fraction::operator - (const Fraction &F) const
{
	Fraction Result(F.Y / std::__gcd(Y, F.Y) * X - Y / std::__gcd(Y, F.Y) * F.X, Y / std::__gcd(Y, F.Y) * F.Y);
	int d = std::__gcd(Result.X, Result.Y);
	Result.X /= d;
	Result.Y /= d;
	return Result;
}

Fraction Fraction::operator -= (const Fraction &F)
{
	*this = *this - F;
	return *this;
}

double Fraction::toDouble() const
{
	return (double) X / (double) Y;
}

int ran()
{
	static int X = rand();
	X += (X << 2) ^ 137;
	return X;
}
