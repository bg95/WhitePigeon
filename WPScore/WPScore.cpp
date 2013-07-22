#include "WPScore.h"

WPScore::WPScore()
{
	NoteList.clear();
}

WPScore::~WPScore()
{
}

void WPScore::save(const std::string &FileName)
{
}

void WPScore::load(const std::string &FileName)
{
}

void WPScore::insertNote(const WPPosition &P, const WPNote &N)
{
	NoteList.push_back(std::make_pair(P, N));
	for (int k = (int) NoteList.size() - 1; k > 0; -- k)
		if (NoteList[k].first.getValue() < NoteList[k - 1].first.getValue())
			std::swap(NoteList[k], NoteList[k - 1]);
}

void WPScore::deleteNote(const WPPosition &P, const WPNote &N)
{
}

WPProperty WPScore::insertProperty(const WPInterval &I, const WPProperty &P)
{
	return P;
}

void WPScore::deleteProperty(const WPProperty &)
{
}

void WPScore::insertPart(const WPPart &P)
{
	PartList.push_back(P);
}

void WPScore::deletePart(const WPPart &)
{
}

std::vector <WPPart> WPScore::getPartList() const
{
	return std::vector <WPPart> ();
}

std::vector < std::pair <WPPosition, WPNote> > WPScore::getNoteList() const
{
	return NoteList;
}

std::vector <WPProperty> WPScore::getPropertyList() const
{
	return std::vector <WPProperty> ();
}
