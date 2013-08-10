#include "WPPart.h"

WPPart::WPPart (WPScore *M)
{
	Master = M;
	IsToBePlayed = 1;
	IsDeleted = 0;
	PlayingPosition = WPPosition (Fraction (0, 1));
	VerMap.clear();
	MasterVer = Master->getCurrentVersion();
	VerMap[MasterVer] = 0;
	MyVer = 0;
	Names.clear();
	Names.push_back("");
	Notes.clear();
	Notes.push_back(WPMultinotePersistentTree (Master->getMultinotePersistentTreeNodeAllocator()));
	Properties.clear();
	Properties.push_back(WPPropertyPersistentTree (Master->getPropertyPersistentTreeNodeAllocator()));
}

WPPart::WPPart (WPScore *M, const std::string &S)
{
	Master = M;
	IsToBePlayed = 1;
	IsDeleted = 0;
	PlayingPosition = WPPosition (Fraction (0, 1));
	VerMap.clear();
	MasterVer = Master->getCurrentVersion();
	VerMap[MasterVer] = 0;
	MyVer = 0;
	Names.clear();
	Names.push_back(S);
	Notes.clear();
	Notes.push_back(WPMultinotePersistentTree (Master->getMultinotePersistentTreeNodeAllocator()));
	Properties.clear();
	Properties.push_back(WPPropertyPersistentTree (Master->getPropertyPersistentTreeNodeAllocator()));
}

WPPart::~WPPart ()
{
}

void WPPart::synchronizeWithMaster()
{
	MasterVer = Master->getCurrentVersion();
	std::map <int, int> :: iterator it = VerMap.upper_bound(MasterVer);
	if (it == VerMap.begin())
		MyVer = - 1;
	else
	{
		-- it;
		MyVer = it->second;
	}
}

void WPPart::implementVersion()
{
	synchronizeWithMaster();
	if (!VerMap.count(MasterVer))
	{
		VerMap[MasterVer] = VerMap.size();
		Names.push_back(Names[MyVer]);
		Notes.push_back(Notes[MyVer]);
		Properties.push_back(Properties[MyVer]);
		MyVer = VerMap[MasterVer];
	}
}

std::string WPPart::getName()
{
	synchronizeWithMaster();
	return Names[MyVer];
}

void WPPart::insertProperties(const WPProperty &P)
{
	implementVersion();
	Properties[MyVer].insert(P);
}

void WPPart::insertMultinote(const WPPosition &P, const WPMultinote &N)
{
	implementVersion();
	Notes[MyVer].insert(P, N);
	// Need to change properties;
}

std::pair < Fraction, std::vector <WPProperty> > WPPart::startFrom(const WPPosition &P)
{
	synchronizeWithMaster();
	std::vector <WPProperty> V = Properties[MyVer].query(WPInterval (WPPosition (Fraction (0, 1)), P)).traverse();
	std::vector <WPProperty> Result;
	for (std::vector <WPProperty> :: iterator it = V.begin(); it != V.end(); ++ it)
		if (it->getInterval().end() > P)
			Result.push_back(*it);
	PlayingPosition = P;
	return std::make_pair(Notes[MyVer].query(P).first, Result);
}

std::pair < WPMultinote, std::pair < std::vector <WPProperty>, std::vector <WPProperty> > > WPPart::nextFragment()
{
	synchronizeWithMaster();
	WPMultinote Nt = Notes[MyVer].query(PlayingPosition).second;
	std::vector <WPProperty> Starting = Properties[MyVer].query(WPInterval (PlayingPosition, PlayingPosition + Nt.getLength())).traverse();
	std::vector <WPProperty> V = Properties[MyVer].query(WPInterval (WPPosition (Fraction (0, 1)), PlayingPosition + Nt.getLength())).traverse();
	std::vector <WPProperty> Ending;
	for (std::vector <WPProperty> :: iterator it = V.begin(); it != V.end(); ++ it)
		if (it->getInterval().end() > PlayingPosition && it->getInterval().end() <= PlayingPosition + Nt.getLength())
			Ending.push_back(*it);
	PlayingPosition += Nt.getLength();
	return std::make_pair(Nt, std::make_pair(Starting, Ending));
}

//~ void WPPart::insertMultinote(const WPPosition &P, const WPMultinote &N)
//~ {
	//~ std::vector <WPMultinote> New;
	//~ Fraction Past(0, 1);
	//~ for (std::vector <WPMultinote> :: iterator it = Notes.begin(); it != Notes.end(); ++ it)
	//~ {
		//~ Fraction NextStop = Past + it->getLength();
		//~ if (NextStop <= P.getValue())
			//~ New.push_back(*it);
		//~ else
		//~ {
			//~ if (Past == P.getValue())
			//~ {
				//~ New.push_back(N);
				//~ New.push_back(*it);
			//~ }
			//~ else
			//~ {
				//~ if (Past < P.getValue())
				//~ {
					//~ WPMultinote Cpy = *it;
					//~ Cpy.setLength(P.getValue() - Past);
					//~ New.push_back(Cpy);
					//~ New.push_back(N);
					//~ Cpy.setLength(it->getLength() - Cpy.getLength());
					//~ New.push_back(Cpy);
				//~ }
				//~ else
					//~ New.push_back(*it);
			//~ }
		//~ }
		//~ Past = NextStop;
	//~ }
	//~ if (Past == P.getValue())
		//~ New.push_back(N);
	//~ else
	//~ {
		//~ if (Past < P.getValue())
		//~ {
			//~ New.push_back(WPMultinote (WPNote (WPNote::Rest, P.getValue() - Past)));
			//~ New.push_back(N);
		//~ }
	//~ }
	//~ Notes = New;
	//~ for (std::vector <WPProperty> :: iterator it = Properties.begin(); it != Properties.end(); ++ it)
	//~ {
		//~ if (it->getInterval().begin() < P && P < it->getInterval().end())
			//~ it->lengthen(N.getLength());
		//~ else
		//~ {
			//~ if (P <= it->getInterval().begin())
				//~ it->shiftRight(N.getLength());
		//~ }
	//~ }
//~ }
