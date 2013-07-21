#include "WPPart.h"

WPPart::WPPart ()
{
}

WPPart::~WPPart ()
{
}

void WPPart::insertProperties(const WPProperty &P)
{
	Properties.push_back(P);
}
