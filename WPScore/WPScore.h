#ifndef _WPSCORE_H
#define _WPSCORE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <QByteArray>
#include <QReadWriteLock>

class WPScore;

#include "WPNote.h"
#include "WPPosition.h"
#include "WPInterval.h"
#include "WPProperty.h"
#include "WPPart.h"

class WPScore: public QReadWriteLock
{
	public:
		static const int FailToOpenFile;
		static const int FileIncomplete;
		WPScore();
		~WPScore();
		void save(const std::string &);
		int load(const std::string &);
		void close();
		WPPart *newPart();
		WPPart *newPart(std::string);
		std::vector <WPPart> &getPartList();
		//~ std::vector <WPProperty> getPropertyList() const;
		int countPartNumber();
		WPPart *getPartByOrder(const int &);
		void insertProperty(const WPProperty &); // remember to call newVersion() first!
		bool deleteProperty(const WPProperty &); // remember to call newVersion() first!
		std::vector <WPProperty> getAllProperties();
		std::vector <WPProperty> filterPropertiesByPrefix(const std::string &);
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
		std::vector <WPPropertyPersistentTree> Properties;
		WPAllocator <WPMultinotePersistentTreeNode> MPTNAlloc;
		WPAllocator <WPPropertyPersistentTreeNode> PPTNAlloc;
		void AnalysisScore0a(const std::string &);
		void AnalysisPart0a(WPPart *, const std::string &);
		WPMultinote AnalysisMultinote0a(const std::string &);
		WPNote AnalysisNote0a(const std::string &);
		WPProperty AnalysisProperty0a(const std::string &);
		WPInterval AnalysisInterval0a(const std::string &);
};

#endif
