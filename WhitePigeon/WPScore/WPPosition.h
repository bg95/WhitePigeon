#ifndef _WPPOSITION_H
#define _WPPOSITION_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "WPLib.h"

class WPPosition
{
	public:
		WPPosition();
		~WPPosition();
		WPPosition(const Fraction &);
		Fraction getValue() const;
		bool operator < (const WPPosition &) const;
		bool operator <= (const WPPosition &) const;
		bool operator > (const WPPosition &) const;
		bool operator >= (const WPPosition &) const;
		bool operator == (const WPPosition &) const;
		WPPosition operator = (const Fraction &);
		WPPosition operator + (const Fraction &) const;
		WPPosition operator += (const Fraction &);
	private:
		Fraction Value;
};

#endif

