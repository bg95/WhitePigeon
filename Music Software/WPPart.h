#ifndef _WPPART_H
#define _WPPART_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "WPMultinote.h"
#include "WPProperty.h"

class WPPart
{
	public:
		WPPart();
		~WPPart();
		
	private:
		std::vector <WPMultinote> Notes;
		std::vector <WPProperty> Properties;
};

#endif

