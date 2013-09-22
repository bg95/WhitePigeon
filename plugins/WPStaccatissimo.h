#ifndef WPSTACCATISSIMO_H
#define WPSTACCATISSIMO_H

#include "WPModifierPlugin.h"

class WPStaccatissimo : public WPModifierPlugin
{
public:
    WPStaccatissimo();
    ~WPStaccatissimo();

    bool isNoteModifier()
    {
        return true;
    }
    bool isAmpModifier()
    {
        return true;
    }

    void setNotes(WPMultinote *notes, int num, double offset);

    double modifyNote(double time); //if a note starts at (near) time, returns the length, otherwise returns a negative
    std::vector<double> modifyAmp(double time, std::vector<double> amp);

private:
    const double Shortened = 0.25;
    std::vector<double> notestart;

};

#endif // WPSTACCATISSIMO_H
