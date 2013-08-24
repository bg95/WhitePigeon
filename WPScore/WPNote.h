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
		static const int Rest;
		WPNote();
		WPNote(const int &, const Fraction &);
		~WPNote();
		bool operator == (const WPNote &) const;
		int getPitch() const;
		double getFrequency() const;
		double getFrequency(double *(int, int), int) const;
		Fraction getLength() const;
		double getTimeSpan() const;
		void setLength(const Fraction &);
	private:
		Fraction Length;
		int Pitch; /// 0 = 440 Hz
};

#endif

