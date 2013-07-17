#ifndef _WPINTERVAL_H
#define _WPINTERVAL_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "WPPosition.h"

class WPInterval
{
	public:
		WPInterval();
		WPInterval(const WPPosition &, const WPPosition &);
		~WPInterval();
		WPPosition begin() const;
		WPPosition end() const;
	private:
		WPPosition Begin, End;
};

#endif

