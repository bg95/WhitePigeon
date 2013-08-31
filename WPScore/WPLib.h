#ifndef _WPLIB_H
#define _WPLIB_H

#include <algorithm>
#include <iostream>
#include <fstream>
#include <climits>
#include <cstdlib>
#include <vector>
#include <cstdio>
#include <cmath>

template <class T> T checkmax(T &, T);
template <class T> T checkmin(T &, T);
template <class T> T GCD(T, T);

template <class T> T checkmax(T &a, T b)
{
	return (a > b) ? a : a = b;
}

template <class T> T checkmin(T &a, T b)
{
	return (a < b) ? a : a = b;
}

template <class T> T GCD(T a, T b)
{
	if (a < 0)
		a = - a;
	if (b < 0)
		b = - b;
	while (a != 0)
	{
		T c = b % a;
		b = a;
		a = c;
	}
	return b;
}

class Fraction
{
	public:
		Fraction();
		Fraction(const double &);
		Fraction(const int &);
		Fraction(const int &, const int &);
		~Fraction();
		bool operator == (const Fraction &) const;
		bool operator < (const Fraction &) const;
		bool operator <= (const Fraction &) const;
		bool operator > (const Fraction &) const;
		bool operator >= (const Fraction &) const;
		Fraction operator = (const Fraction &);
		Fraction operator + (const Fraction &) const;
		Fraction operator += (const Fraction &);
		Fraction operator - (const Fraction &) const;
		Fraction operator -= (const Fraction &);
		Fraction operator * (const Fraction &) const;
		Fraction operator *= (const Fraction &);
		Fraction operator / (const Fraction &) const;
		Fraction operator /= (const Fraction &);
		Fraction inverse() const;
		double toDouble() const;
		int X, Y;
};

Fraction stringToFraction(const std::string &);
Fraction doubleToFraction(const double &); // warning!!!
std::string intToString(const int &);
std::string fractionToString(const Fraction &);
int ran();

#endif
