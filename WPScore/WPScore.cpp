#include "WPScore.h"

const int WPScore::FailToOpenFile = - 1;
const int WPScore::FileIncomplete = - 2;

WPScore::WPScore()
{
	PartList.clear();
	CurVer = 0;
	VerLink.clear();
	VerLink.push_back(std::make_pair(- 1, - 1));
	Properties.clear();
	Properties.push_back(WPPropertyPersistentTree (getPropertyPersistentTreeNodeAllocator()));
}

WPScore::~WPScore()
{
	
}

void WPScore::save(const std::string &FileName)
{
	FILE *fo = fopen(FileName.c_str(), "wb");
	fprintf(fo, "<version>Score File Format 0b</version>");
	QByteArray BA;
	BA.append("<versioninfo>All rights reserved.</versioninfo>");
	for (int k = 0; k < countPartNumber(); ++ k)
	{
		WPPart *P = getPartByOrder(k);
		if (!P)
		{
			fprintf(stderr, "error when saving!\n");
			continue;
		}
		BA.append("<newpart>");
		BA.append(P->getName().c_str());
		BA.append("</newpart>");
		std::vector <WPMultinote> Nt = P->getAllNotes();
		BA.append("<part>");
		for (std::vector <WPMultinote> :: iterator it = Nt.begin(); it != Nt.end(); ++ it)
		{
			BA.append("<multinote>");
			std::vector <WPNote> Nts = it->getNotes();
			for (std::vector <WPNote> :: iterator eit = Nts.begin(); eit != Nts.end(); ++ eit)
				BA.append(("<note><pitch>" + intToString(eit->getPitch()) + "</pitch><length>" + fractionToString(eit->getLength()) + "</length></note>").c_str());
			BA.append("</multinote>");
		}
		std::vector <WPProperty> Pp = P->getAllProperties();
		for (std::vector <WPProperty> :: iterator it = Pp.begin(); it != Pp.end(); ++ it)
			BA.append(("<property><interval><position>" + fractionToString(it->getInterval().begin().getValue()) + "</position><position>" + fractionToString(it->getInterval().end().getValue()) + "</position></interval><arg>" + it->getArg() + "</arg></property>").c_str());
		BA.append("</part>");
	}
	std::vector <WPProperty> Pp = getAllProperties();
	for (std::vector <WPProperty> :: iterator it = Pp.begin(); it != Pp.end(); ++ it)
		BA.append(("<property><interval><position>" + fractionToString(it->getInterval().begin().getValue()) + "</position><position>" + fractionToString(it->getInterval().end().getValue()) + "</position></interval><arg>" + it->getArg() + "</arg></property>").c_str());
	BA = qCompress(BA, 9);
	int NS = BA.size();
	fprintf(stderr, "size = %u\n", BA.size());
	fwrite(BA.data(), NS, 1, fo);
	fclose(fo);
}

int WPScore::load(const std::string &FileName)
{
	FILE *fi = fopen(FileName.c_str(), "rb");
	if (fi == NULL)
		return WPScore::FailToOpenFile;
	std::string V = "";
	int Ch;
	const int Version_Size = 9; // length of "<version>"
	while ((Ch = getc(fi)) != EOF)
	{
		V.push_back(Ch);
		if ((int) V.size() > Version_Size && "</version>" == V.substr(V.size() - (Version_Size + 1), Version_Size + 1))
			break;
	}
	if (Ch == EOF)
		return WPScore::FileIncomplete;
	V = V.substr(Version_Size, V.size() - (Version_Size * 2 + 1));
	std::string Rest = "";
	while ((Ch = getc(fi)) != EOF)
		Rest.push_back(Ch);
	fclose(fi);
	if (V == "Score File Format 0a")
		AnalysisScore0a(Rest);
	if (V == "Score File Format 0b")
	{
		QByteArray BA = QByteArray::fromRawData(Rest.c_str(), Rest.size());
		BA = qUncompress(BA);
		Rest = BA.data();
		AnalysisScore0a(Rest);
	}
	return 0;
}

void WPScore::close()
{
	MPTNAlloc.Recycle();
	PPTNAlloc.Recycle();
	PartList.clear();
	CurVer = 0;
	VerLink.clear();
	VerLink.push_back(std::make_pair(- 1, - 1));
	Properties.clear();
	Properties.push_back(WPPropertyPersistentTree (getPropertyPersistentTreeNodeAllocator()));
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

void WPScore::insertProperty(const WPProperty &P)
{
	Properties[CurVer].insert(P);
}

bool WPScore::deleteProperty(const WPProperty &P)
{
	return Properties[CurVer].remove(P);
}

std::vector<WPProperty> WPScore::getAllProperties()
{
	return Properties[CurVer].traverse();
}

std::vector<WPProperty> WPScore::filterPropertiesByPrefix(const std::string &S)
{
	return filterPrefix(Properties[CurVer].traverse(), S);
}

int WPScore::getCurrentVersion() const
{
	return CurVer;
}

int WPScore::newVersion()
{
	VerLink[CurVer].second = VerLink.size();
	VerLink.push_back(std::make_pair(CurVer, - 1));
	Properties.push_back(Properties[CurVer]);
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

void WPScore::AnalysisScore0a(const std::string &S)
{
	int PS = 0;
	WPPart *LastPart = NULL;
	for (; PS < (int) S.size(); ++ PS)
		if (S[PS] == '<')
		{
			std::string::size_type FoundPos = S.find('>', PS);
			if (FoundPos == std::string::npos)
				continue;
			int PE = FoundPos;
			int L = PE - PS - 1;
			std::string Tag = S.substr(PS + 1, L);
			FoundPos = S.find("</" + Tag + ">", PE);
			if (FoundPos == std::string::npos)
				continue;
			int PB = FoundPos;
			std::string Inner = S.substr(PE + 1, PB - PE - 1);
			if (Tag == "newpart")
				LastPart = newPart(Inner);
			if (Tag == "part")
			{
				if (LastPart)
					AnalysisPart0a(LastPart, Inner);
			}
			if (Tag == "property")
				insertProperty(AnalysisProperty0a(Inner));
			PS = PB + L + 2;
		}
}

void WPScore::AnalysisPart0a(WPPart *P, const std::string &S)
{
	WPPosition PP(Fraction (0, 1));
	int PS = 0;
	for (; PS < (int) S.size(); ++ PS)
		if (S[PS] == '<')
		{
			std::string::size_type FoundPos = S.find('>', PS);
			if (FoundPos == std::string::npos)
				continue;
			int PE = FoundPos;
			int L = PE - PS - 1;
			std::string Tag = S.substr(PS + 1, L);
			FoundPos = S.find("</" + Tag + ">", PE);
			if (FoundPos == std::string::npos)
				continue;
			int PB = FoundPos;
			std::string Inner = S.substr(PE + 1, PB - PE - 1);
			if (Tag == "multinote")
			{
				WPMultinote MNt = AnalysisMultinote0a(Inner);
				P->insertMultinote(PP, MNt);
				PP += MNt.getLength();
			}
			if (Tag == "property")
				P->insertProperty(AnalysisProperty0a(Inner));
			PS = PB + L + 2;
		}
}

WPMultinote WPScore::AnalysisMultinote0a(const std::string &S)
{
	WPMultinote Result;
	int PS = 0;
	for (; PS < (int) S.size(); ++ PS)
		if (S[PS] == '<')
		{
			std::string::size_type FoundPos = S.find('>', PS);
			if (FoundPos == std::string::npos)
				continue;
			int PE = FoundPos;
			int L = PE - PS - 1;
			std::string Tag = S.substr(PS + 1, L);
			FoundPos = S.find("</" + Tag + ">", PE);
			if (FoundPos == std::string::npos)
				continue;
			int PB = FoundPos;
			std::string Inner = S.substr(PE + 1, PB - PE - 1);
			if (Tag == "note")
				Result.insertNote(AnalysisNote0a(Inner));
			PS = PB + L + 2;
		}
	return Result;
}

WPNote WPScore::AnalysisNote0a(const std::string &S)
{
	int Pitch = WPNote::Rest;
	Fraction Length(- 1, 1);
	int PS = 0;
	for (; PS < (int) S.size(); ++ PS)
		if (S[PS] == '<')
		{
			std::string::size_type FoundPos = S.find('>', PS);
			if (FoundPos == std::string::npos)
				continue;
			int PE = FoundPos;
			int L = PE - PS - 1;
			std::string Tag = S.substr(PS + 1, L);
			FoundPos = S.find("</" + Tag + ">", PE);
			if (FoundPos == std::string::npos)
				continue;
			int PB = FoundPos;
			std::string Inner = S.substr(PE + 1, PB - PE - 1);
			if (Tag == "pitch")
				Pitch = atoi(Inner.c_str());
			if (Tag == "length")
				Length = stringToFraction(Inner);
			PS = PB + L + 2;
		}
	return WPNote (Pitch, Length);
}

WPProperty WPScore::AnalysisProperty0a(const std::string &S)
{
	WPInterval Interval(WPPosition (Fraction (0, 1)), WPPosition (Fraction (1, 0)));
	std::string Arg;
	int PS = 0;
	for (; PS < (int) S.size(); ++ PS)
		if (S[PS] == '<')
		{
			std::string::size_type FoundPos = S.find('>', PS);
			if (FoundPos == std::string::npos)
				continue;
			int PE = FoundPos;
			int L = PE - PS - 1;
			std::string Tag = S.substr(PS + 1, L);
			FoundPos = S.find("</" + Tag + ">", PE);
			if (FoundPos == std::string::npos)
				continue;
			int PB = FoundPos;
			std::string Inner = S.substr(PE + 1, PB - PE - 1);
			if (Tag == "arg")
				Arg = Inner;
			if (Tag == "interval")
				Interval = AnalysisInterval0a(Inner);
			PS = PB + L + 2;
		}
	return WPProperty (Interval, Arg);
}

WPInterval WPScore::AnalysisInterval0a(const std::string &S)
{
	WPPosition St(Fraction (0, 1)), Tt(Fraction (1, 0));
	int Cnt = 0;
	int PS = 0;
	for (; PS < (int) S.size(); ++ PS)
		if (S[PS] == '<')
		{
			std::string::size_type FoundPos = S.find('>', PS);
			if (FoundPos == std::string::npos)
				continue;
			int PE = FoundPos;
			int L = PE - PS - 1;
			std::string Tag = S.substr(PS + 1, L);
			FoundPos = S.find("</" + Tag + ">", PE);
			if (FoundPos == std::string::npos)
				continue;
			int PB = FoundPos;
			std::string Inner = S.substr(PE + 1, PB - PE - 1);
			if (Tag == "position")
			{
				if (Cnt == 0)
					St = WPPosition (stringToFraction(Inner));
				if (Cnt == 1)
					Tt = WPPosition (stringToFraction(Inner));
				++ Cnt;
			}
			PS = PB + L + 2;
		}
	return WPInterval (St, Tt);
}
