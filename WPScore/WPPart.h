#ifndef _WPPART_H
#define _WPPART_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "WPPersistentTree.h"

class WPPart;

#include "WPScore.h"

class WPPart
{
	public:
		WPPart(WPScore *);
		WPPart(WPScore *, const std::string &);
		~WPPart();
		std::string getName();
		void insertProperties(const WPProperty &);
		void insertMultinote(const WPPosition &, const WPMultinote &);
		void synchronizeWithMaster();
		void implementVersion();
		std::pair < Fraction, std::vector <WPProperty> > startFrom(const WPPosition &);
		std::pair < WPMultinote, std::pair < std::vector <WPProperty>, std::vector <WPProperty> > > nextFragment();
	private:
		WPScore *Master;
		int MasterVer, MyVer;
		bool IsToBePlayed, IsDeleted;
		WPPosition PlayingPosition;
		std::map <int, int> VerMap;
		std::vector <std::string> Names;
		std::vector <WPMultinotePersistentTree> Notes;
		std::vector <WPPropertyPersistentTree> Properties;
};

#endif

