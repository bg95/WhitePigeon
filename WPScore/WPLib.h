#ifndef _WPLIB_H
#define _WPLIB_H

#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <cmath>

template <class T> T checkmax(T &a, T b);
template <class T> T checkmin(T &a, T b);

template <class T> T checkmax(T &a, T b)
{
	return (a > b) ? a : a = b;
}

template <class T> T checkmin(T &a, T b)
{
	return (a < b) ? a : a = b;
}

class Fraction
{
	public:
		Fraction();
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
std::string intToString(const int &);
std::string fractionToString(const Fraction &);
int ran();

#endif
