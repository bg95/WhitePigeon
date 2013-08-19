#ifndef WPSTACCATISSIMO_H
#define WPSTACCATISSIMO_H

#include "WPModifier.h"

class WPStaccatissimo : public WPModifier
{
public:
    WPStaccatissimo();

    bool isGlobal()
    {
        return false;
    }
    bool isSingleNote()
    {
        return true;
    }
    bool isNoteModifier()
    {
        return true;
    }
    bool isAmpModifier()
    {
        return true;
    }

    void setNotes(const std::vector<WPNote> &_notes);

    double modifyNote(double time);
    double modifyAmp(double time, double amp);

private:
    const double Shortened = 0.25;
    double length;

};

#endif // WPSTACCATISSIMO_H
