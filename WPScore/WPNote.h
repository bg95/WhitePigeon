#ifndef _WPNOTE_H
#define _WPNOTE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "WPLib.h"

class WPNote
{
	public:
		static const int Rest = - 999999;
		WPNote();
		WPNote(const int &, const Fraction &);
		~WPNote();
		bool operator == (const WPNote &) const;
		int getPitch() const;
		double getFrequency() const;
		Fraction getLength() const;
		double getTimeSpan() const;
	private:
		Fraction Length;
		int Pitch; /// 0 = 440 Hz
};

#endif

