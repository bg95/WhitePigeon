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
		void insertProperties(const WPProperty &); // remember to call newVersion() first!
		void insertMultinote(const WPPosition &, const WPMultinote &); // remember to call newVersion() first!
		void synchronizeWithMaster();
		void implementVersion();
		void newVersion();
		void setToBeMuted();
		void setToBePlayed();
		bool isToBePlayed();
		int displayOrder(); // -1 means deleted.
		void setOrder(const int &);
		std::vector <WPMultinote> getAllNotes();
		std::vector <WPProperty> getAllProperties();
		std::pair < Fraction, std::vector <WPProperty> > startFrom(const WPPosition &);
		std::pair < WPMultinote, std::pair < std::vector <WPProperty>, std::vector <WPProperty> > > nextFragment();
	private:
		WPScore *Master;
		int MasterVer, MyVer;
		bool IsToBePlayed;
		WPPosition PlayingPosition;
		std::map <int, int> VerMap;
		std::vector <std::string> Names;
		std::vector <int> Orders;
		std::vector <WPMultinotePersistentTree> Notes;
		std::vector <WPPropertyPersistentTree> Properties;
};

#endif

