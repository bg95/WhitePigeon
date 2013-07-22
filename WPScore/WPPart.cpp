#include "WPPart.h"

WPPart::WPPart ()
{
}

WPPart::WPPart (const std::string &S)
{
	Name = S;
}

WPPart::~WPPart ()
{
}

std::string WPPart::getName() const
{
	return Name;
}

void WPPart::insertProperties(const WPProperty &P)
{
	Properties.push_back(P);
}
