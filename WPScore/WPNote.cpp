#include "WPNote.h"

WPNote::WPNote ()
{
}

WPNote::~WPNote ()
{
}

WPNote::WPNote (const int &P, const Fraction &L)
{
	Pitch = P;
	Length = L;
}

bool WPNote::operator == (const WPNote &Nt) const
{
	return Length == Nt.Length && Pitch == Nt.Pitch;
}

int WPNote::getPitch() const
{
	return Pitch;
}

double WPNote::getFrequency() const
{
	return 440.0 * pow(2.0, (double) Pitch / 12.0);
}

double WPNote::getFrequency(double *TuningFunction(int, int), int Base) const
{
	return *TuningFunction(Base, Pitch);
}

Fraction WPNote::getLength() const
{
	return Length;
}

double WPNote::getTimeSpan() const
{
	return Length.toDouble();
}
