#include "WPLib.h"

Fraction::Fraction()
{
}

Fraction::~Fraction()
{
}

Fraction::Fraction(const double &D)
{
#pragma message("Caution! Please be careful transferring a real number to a rational number! It might cause fatal error!")
	std::vector <int> frac;
	long double a = D;
	double acceptableEps = 1.0;
	for (int i = 1; i <= 16; ++ i)
	{
		if (fabsl(floorl(a)) > INT_MAX)
			break;
		int b = floorl(a);
		frac.push_back(b);
		long double c = a - b;
		a = 1.0 / c;
		std::reverse(frac.begin(), frac.end());
		Fraction Tmp(1, 0);
		for (std::vector <int> :: iterator it = frac.begin(); it != frac.end(); ++ it)
			Tmp = Tmp.inverse() + *it;
		std::reverse(frac.begin(), frac.end());
		if (fabs(Tmp.toDouble() - D) > acceptableEps)
		{
			frac.pop_back();
			break;
		}
		acceptableEps = fabs(Tmp.toDouble() - D);
	}
	std::reverse(frac.begin(), frac.end());
	X = 1;
	Y = 0;
	for (std::vector <int> :: iterator it = frac.begin(); it != frac.end(); ++ it)
		*this = this->inverse() + *it;
}

Fraction::Fraction(const int &I)
{
	X = I;
	Y = 1;
}

Fraction::Fraction(const int &A, const int &B)
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
	Fraction Result(F.Y / GCD(Y, F.Y) * X + Y / GCD(Y, F.Y) * F.X, Y / GCD(Y, F.Y) * F.Y);
	int d = GCD(Result.X, Result.Y);
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
	Fraction Result(F.Y / GCD(Y, F.Y) * X - Y / GCD(Y, F.Y) * F.X, Y / GCD(Y, F.Y) * F.Y);
	int d = GCD(Result.X, Result.Y);
	Result.X /= d;
	Result.Y /= d;
	return Result;
}

Fraction Fraction::operator -= (const Fraction &F)
{
	*this = *this - F;
	return *this;
}

Fraction Fraction::operator * (const Fraction &F) const
{
	Fraction Result((long long) F.X / GCD(F.X, Y) * X / GCD(X, F.Y), (long long) Y / GCD(F.X, Y) * F.Y / GCD(X, F.Y));
	int d = GCD(Result.X, Result.Y);
	Result.X /= d;
	Result.Y /= d;
	return Result;
}

Fraction Fraction::operator *= (const Fraction &F)
{
	*this = *this * F;
	return *this;
}

Fraction Fraction::operator / (const Fraction &F) const
{
	return *this * F.inverse();
}

Fraction Fraction::operator /=(const Fraction &F)
{
	*this = *this / F;
	return *this;
}

Fraction Fraction::inverse() const
{
	int A = X;
	int B = Y;
	if (B < 0)
	{
		A = - A;
		B = - B;
	}
	return Fraction (B, A);
}

double Fraction::toDouble() const
{
	return (double) X / (double) Y;
}

Fraction stringToFraction(const std::string &S)
{
	int X, Y;
	X = Y = 0;
	sscanf(S.c_str(), "%d/%d", &X, &Y);
	return Fraction (X, Y);
}

int ran()
{
	static int X = rand();
	X += (X << 2) ^ 137;
	return X;
}


std::string intToString(const int &I)
{
	char St[50];
	sprintf(St, "%d", I);
	return St;
}


std::string fractionToString(const Fraction &F)
{
	return intToString(F.X) + "/" + intToString(F.Y);
}


Fraction doubleToFraction(const double &D)
{
	return D;
}
