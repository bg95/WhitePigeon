#ifndef _WPSCORE_H
#define _WPSCORE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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
		void insertNote(const WPPosition &, const WPNote &);
		void deleteNote(const WPPosition &, const WPNote &);
		WPProperty insertProperty(const WPInterval &, const WPProperty &);
		void deleteProperty(const WPProperty &);
		void insertPart(const WPPart &);
		void deletePart(const WPPart &);
		std::vector <WPPart> getPartList() const;
		std::vector < std::pair <WPPosition, WPNote> > getNoteList() const;
		std::vector <WPProperty> getPropertyList() const;
	private:
		std::vector < std::pair <WPPosition, WPNote> > NoteList;
		std::vector <WPPart> PartList;
};

#endif
