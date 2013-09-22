#ifndef _WPPROPERTY_H
#define _WPPROPERTY_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <QString>
#include <QByteArray>
#include "WPInterval.h"

/*
 * By default, we use percent-encoding for argument(s).
 * This setting makes our saving easy.
 * Also, we can write our function much easily.
 */

class WPProperty
{
	public:
		WPProperty();
		WPProperty(const WPInterval &, const std::string &);
		~WPProperty();
		WPInterval getInterval() const;
		std::string getArg() const;
		void lengthen(const Fraction &);
		void shiftRight(const Fraction &);
		bool operator < (const WPProperty &) const;
		QString setEntry(const std::string &, const QString &); // return the old value.
		void deleteEntry(const std::string &);
		bool existEntry(const std::string &) const;
		QString getEntry(const std::string &) const;
	private:
		WPInterval Interval;
		std::string Arg;
		std::size_t FindEntry(const std::string &) const;
};

std::vector <WPProperty> filterPrefix(const std::vector <WPProperty> &, const std::string &);

#endif

