#ifndef WPSTACCATISSIMO_H
#define WPSTACCATISSIMO_H

#include "WPModifier.h"

class WPStaccatissimo : public WPModifier
{
public:
    WPStaccatissimo();
    ~WPStaccatissimo();

    bool isNoteModifier()
    {
        return true;
    }
    Precedence isAmpModifier()
    {
        return PROP;
    }

	NotesRequirement needNotes()
	{
		return SINGLE;
	}
    //void setNotes(WPMultinote *notes, int num, double offset);
	void reset();

    double modifyNote(double time); //if a note starts at (near) time, returns the length, otherwise returns a negative
    std::vector<double> modifyAmp(double time, std::vector<double> amp);

private:
    const double Shortened;
	double length;
    //std::vector<double> notestart;

};

#endif // WPSTACCATISSIMO_H
