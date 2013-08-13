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
		Fraction(int, int);
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
		double toDouble() const;
		int X, Y;
};

Fraction stringToFraction(const std::string &);
int ran();

#endif
