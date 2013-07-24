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
		enum WPPropertyType
		{
			Staccato
		};
		WPProperty();
		WPProperty(const WPInterval &, const WPPropertyType &, const std::string &);
		~WPProperty();
		WPInterval getInterval() const;
		WPPropertyType getType() const;
		std::string getArg() const;
		void lengthen(const Fraction &);
		void shiftRight(const Fraction &);
	private:
		WPInterval Interval;
		WPPropertyType Type;
		std::string Arg;
};

#endif

