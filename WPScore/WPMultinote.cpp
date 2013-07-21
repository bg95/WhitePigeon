#include "WPMultinote.h"

WPMultinote::WPMultinote()
{
}

WPMultinote::WPMultinote(const WPNote &Nt)
{
	List = std::vector <WPNote> (1, Nt);
}

WPMultinote::~WPMultinote()
{
}

void WPMultinote::insertNote(const WPNote &Nt)
{
	List.push_back(Nt);
}

void WPMultinote::change(const std::vector <WPNote> &Vector)
{
	List = Vector;
}

void WPMultinote::change(const WPMultinote &Mlt)
{
	List = Mlt.List;
}

void WPMultinote::deleteNote(const WPNote &Nt)
{
	for (std::vector <WPNote> :: iterator it = List.begin(); it != List.end(); ++ it)
		if (*it == Nt)
		{
			List.erase(it);
			break;
		}
}

std::vector <WPNote> WPMultinote::getNotes()
{
	return List;
}
