#include "WPScore.h"

WPScore::WPScore()
{
	PartList.clear();
	CurVer = 0;
	VerLink.clear();
	VerLink.push_back(std::make_pair(- 1, - 1));
}

WPScore::~WPScore()
{
}

void WPScore::save(const std::string &FileName)
{
	FILE *fo = fopen(FileName.c_str(), "w");
	fprintf(fo, "<version>Score File Format 0a</version><versioninfo>All rights reserved.</versioninfo>");
	for (int k = 0; k < countPartNumber(); ++ k)
	{
		WPPart *P = getPartByOrder(k);
		if (!P)
		{
			fprintf(stderr, "error when saving!\n");
			continue;
		}
		fprintf(fo, "<newpart>%s</newpart>", P->getName().c_str());
		std::vector <WPMultinote> Nt = P->getAllNotes();
		fprintf(fo, "<part>");
		for (std::vector <WPMultinote> :: iterator it = Nt.begin(); it != Nt.end(); ++ it)
		{
			fprintf(fo, "<multinote>");
			std::vector <WPNote> Nts = it->getNotes();
			for (std::vector <WPNote> :: iterator eit = Nts.begin(); eit != Nts.end(); ++ eit)
				fprintf(fo, "<note><pitch>%d</pitch><length>%d/%d</length></note>", eit->getPitch(), eit->getLength().X, eit->getLength().Y);
			fprintf(fo, "</multinote>");
		}
		fprintf(fo, "</part>");
	}
	fclose(fo);
}

void WPScore::load(const std::string &FileName)
{
}

WPPart *WPScore::newPart()
{
	int Number = countPartNumber();
	PartList.push_back(WPPart (this));
	PartList[PartList.size() - 1].setOrder(Number);
	return &PartList[PartList.size() - 1];
}

WPPart *WPScore::newPart(std::string S)
{
	int Number = countPartNumber();
	PartList.push_back(WPPart (this, S));
	PartList[PartList.size() - 1].setOrder(Number);
	return &PartList[PartList.size() - 1];
}

//~ void WPScore::insertNote(const WPPosition &P, const WPNote &N)
//~ {
	//~ NoteList.push_back(std::make_pair(P, N));
	//~ for (int k = (int) NoteList.size() - 1; k > 0; -- k)
		//~ if (NoteList[k].first.getValue() < NoteList[k - 1].first.getValue())
			//~ std::swap(NoteList[k], NoteList[k - 1]);
//~ }
//~ 
//~ void WPScore::deleteNote(const WPPosition &P, const WPNote &N)
//~ {
//~ }
//~ 
//~ WPProperty WPScore::insertProperty(const WPInterval &I, const WPProperty &P)
//~ {
	//~ return P;
//~ }
//~ 
//~ void WPScore::deleteProperty(const WPProperty &)
//~ {
//~ }
//~ 
//~ void WPScore::insertPart(const WPPart &P)
//~ {
	//~ PartList.push_back(P);
//~ }
//~ 
//~ void WPScore::deletePart(const WPPart &)
//~ {
//~ }
//~ 
std::vector <WPPart> &WPScore::getPartList()
{
	return PartList;
}
//~ 
//~ std::vector < std::pair <WPPosition, WPNote> > WPScore::getNoteList() const
//~ {
	//~ return NoteList;
//~ }
//~ 
//~ std::vector <WPProperty> WPScore::getPropertyList() const
//~ {
	//~ return std::vector <WPProperty> ();
//~ }

int WPScore::countPartNumber()
{
	int Result = 0;
	for (std::vector <WPPart> :: iterator it = PartList.begin(); it != PartList.end(); ++ it)
		checkmax(Result, it->displayOrder() + 1);
	return Result;
}

WPPart *WPScore::getPartByOrder(const int &K)
{
	WPPart *Result = NULL;
	for (std::vector <WPPart> :: iterator it = PartList.begin(); it != PartList.end(); ++ it)
		if (it->displayOrder() == K)
			Result = &(*it);
	return Result;
}

int WPScore::getCurrentVersion() const
{
	return CurVer;
}

int WPScore::newVersion()
{
	VerLink[CurVer].second = VerLink.size();
	VerLink.push_back(std::make_pair(CurVer, - 1));
	if (CurVer + 1 != VerLink[CurVer].second)
		for (std::vector <WPPart> :: iterator it = PartList.begin(); it != PartList.end(); ++ it)
			it->synchronizeWithMaster();
	CurVer = VerLink[CurVer].second;
	if (CurVer - 1 != VerLink[CurVer].first)
		for (std::vector <WPPart> :: iterator it = PartList.begin(); it != PartList.end(); ++ it)
			it->implementVersion();
	return getCurrentVersion();
}

void WPScore::switchVersion(const int &V)
{
	if (V >= 0 && V < (int) VerLink.size())
		CurVer = V;
}

int WPScore::undo()
{
	if (VerLink[CurVer].first != - 1)
		CurVer = VerLink[CurVer].first;
	return getCurrentVersion();
}

int WPScore::redo()
{
	if (VerLink[CurVer].second != - 1)
		CurVer = VerLink[CurVer].second;
	return getCurrentVersion();
}

WPAllocator <WPMultinotePersistentTreeNode> *WPScore::getMultinotePersistentTreeNodeAllocator()
{
	return &MPTNAlloc;
}

WPAllocator <WPPropertyPersistentTreeNode> *WPScore::getPropertyPersistentTreeNodeAllocator()
{
	return &PPTNAlloc;
}
