#ifndef _WPPROPERTY_H
#define _WPPROPERTY_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "WPInterval.h"


class WPProperty
{
	public:
		WPProperty();
		WPProperty(const WPInterval &, const std::string &);
		~WPProperty();
		WPInterval getInterval() const;
		std::string getArg() const;
		void lengthen(const Fraction &);
		void shiftRight(const Fraction &);
		bool operator < (const WPProperty &) const;
	private:
		WPInterval Interval;
		std::string Arg;
};

std::vector <WPProperty> filterPrefix(const std::vector <WPProperty> &, const std::string &);

#endif

