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
		//~ void insertNote(const WPPosition &, const WPNote &);
		//~ void deleteNote(const WPPosition &, const WPNote &);
		//~ WPProperty insertProperty(const WPInterval &, const WPProperty &);
		//~ void deleteProperty(const WPProperty &);
		WPPart *newPart();
		WPPart *newPart(std::string);
		void deletePart(WPPart *);
		std::vector <WPPart> getPartList() const;
		//~ std::vector <WPProperty> getPropertyList() const;
		int getCurrentVersion() const;
		WPAllocator <WPMultinotePersistentTreeNode> *getMultinotePersistentTreeNodeAllocator();
		WPAllocator <WPPropertyPersistentTreeNode> *getPropertyPersistentTreeNodeAllocator();
	private:
		int CurVer;
		std::vector <WPPart> PartList;
		WPAllocator <WPMultinotePersistentTreeNode> MPTNAlloc;
		WPAllocator <WPPropertyPersistentTreeNode> PPTNAlloc;
};

#endif
