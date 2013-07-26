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

void WPPart::insertMultinote(const WPPosition &P, const WPMultinote &N)
{
	std::vector <WPMultinote> New;
	Fraction Past(0, 1);
	for (std::vector <WPMultinote> :: iterator it = Notes.begin(); it != Notes.end(); ++ it)
	{
		Fraction NextStop = Past + it->getLength();
		if (NextStop <= P.getValue())
			New.push_back(*it);
		else
		{
			if (Past == P.getValue())
			{
				New.push_back(N);
				New.push_back(*it);
			}
			else
			{
				if (Past < P.getValue())
				{
					WPMultinote Cpy = *it;
					Cpy.setLength(P.getValue() - Past);
					New.push_back(Cpy);
					New.push_back(N);
					Cpy.setLength(it->getLength() - Cpy.getLength());
					New.push_back(Cpy);
				}
				else
					New.push_back(*it);
			}
		}
		Past = NextStop;
	}
	if (Past == P.getValue())
		New.push_back(N);
	else
	{
		if (Past < P.getValue())
		{
			New.push_back(WPMultinote (WPNote (WPNote::Rest, P.getValue() - Past)));
			New.push_back(N);
		}
	}
	Notes = New;
	for (std::vector <WPProperty> :: iterator it = Properties.begin(); it != Properties.end(); ++ it)
	{
		if (it->getInterval().begin() < P && P < it->getInterval().end())
			it->lengthen(N.getLength());
		else
		{
			if (P <= it->getInterval().begin())
				it->shiftRight(N.getLength());
		}
	}
}
