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
	private:
		Fraction Value;
};

#endif

