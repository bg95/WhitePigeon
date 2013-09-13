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
		void lockForRead();
		void lockForWrite();
		void unlock();
		void insertProperty(const WPProperty &); // remember to call newVersion() first!
		bool deleteProperty(const WPProperty &); // remember to call newVersion() first!
		void insertMultinote(const WPPosition &, const WPMultinote &); // remember to call newVersion() first!
		void deleteMultinote(const WPInterval &); // remember to call newVersion() first!
		void synchronizeWithMaster();
		void implementVersion();
		void newVersion();
		void setToBeMuted();
		void setToBePlayed();
		bool isToBePlayed();
		double getVolume();
		int displayOrder(); // -1 means deleted.
		void setOrder(const int &);
		void hide();
		bool isHidden();
		void moveUp(unsigned int Times = 1);
		void moveDown(unsigned int Times = 1);
		WPInterval getExtendedInterval(const WPInterval &);
		WPInterval getReducedInterval(const WPInterval &);
		std::vector <WPMultinote> getAllNotes();
		std::vector <WPProperty> getAllProperties();
		std::vector <WPProperty> filterPropertiesByPrefix(const std::string &);
		std::vector <WPMultinote> getNotesByInterval(WPInterval &);
		std::pair < Fraction, std::vector <WPProperty> > startFrom(const WPPosition &);
		std::pair < WPMultinote, std::pair < std::vector <WPProperty>, std::vector <WPProperty> > > nextFragment();
	private:
		WPScore *Master;
		int MasterVer, MyVer;
		double Volume;
		bool IsToBePlayed;
		WPPosition PlayingPosition;
		std::map <int, int> VerMap;
		std::vector <std::string> Names;
		std::vector <int> Orders;
		std::vector <WPMultinotePersistentTree> Notes;
		std::vector <WPPropertyPersistentTree> Properties;
};

#endif

