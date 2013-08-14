#ifndef _WPSCORE_H
#define _WPSCORE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class WPScore;

#include "WPNote.h"
#include "WPPosition.h"
#include "WPInterval.h"
#include "WPProperty.h"
#include "WPPart.h"

class WPScore
{
	public:
		WPScore();
		~WPScore();
		void save(const std::string &);
		void load(const std::string &);
		void close();
		//~ void insertNote(const WPPosition &, const WPNote &);
		//~ void deleteNote(const WPPosition &, const WPNote &);
		//~ WPProperty insertProperty(const WPInterval &, const WPProperty &);
		//~ void deleteProperty(const WPProperty &);
		WPPart *newPart();
		WPPart *newPart(std::string);
		void deletePart(WPPart *);
		std::vector <WPPart> &getPartList();
		//~ std::vector <WPProperty> getPropertyList() const;
		int countPartNumber();
		WPPart *getPartByOrder(const int &);
		int getCurrentVersion() const;
		int newVersion();
		void switchVersion(const int &);
		int undo(); // WARNING!
		int redo(); // WARNING!
		WPAllocator <WPMultinotePersistentTreeNode> *getMultinotePersistentTreeNodeAllocator();
		WPAllocator <WPPropertyPersistentTreeNode> *getPropertyPersistentTreeNodeAllocator();
	private:
		int CurVer;
		std::vector < std::pair <int, int> > VerLink;
		std::vector <WPPart> PartList;
		WPAllocator <WPMultinotePersistentTreeNode> MPTNAlloc;
		WPAllocator <WPPropertyPersistentTreeNode> PPTNAlloc;
		void AnalysisScore0a(const std::string &);
		void AnalysisPart0a(WPPart *, const std::string &);
		WPMultinote AnalysisMultinote0a(const std::string &);
		WPNote AnalysisNote0a(const std::string &);
};

#endif
