#ifndef _WPLIB_H
#define _WPLIB_H

#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <cmath>

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

#endif
