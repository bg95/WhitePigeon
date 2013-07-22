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
		WPPart(const std::string &);
		~WPPart();
		std::string getName() const;
		void insertProperties(const WPProperty &);
	private:
		std::string Name;
		std::vector <WPMultinote> Notes;
		std::vector <WPProperty> Properties;
};

#endif

