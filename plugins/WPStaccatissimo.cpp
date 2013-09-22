#include "WPStaccatissimo.h"

WPStaccatissimo::WPStaccatissimo()
{
}

WPStaccatissimo::~WPStaccatissimo()
{
}

void WPStaccatissimo::setNotes(WPMultinote *notes, int num, double offset)
{
    WPModifier::setNotes(notes, num, offset);
    int i;
    notestart.clear();
    notestart.push_back(0.0);
    for (i = 0; i < num; i++)
    	notestart.push_back(((notes + i)->*getLengthDouble)() + notestart[i]);
    notestart.pop_back();
}

double WPStaccatissimo::modifyNote(double time)
{
    if (timePassed(0.0))
        return length * Shortened;
    if (timePassed(length * Shortened))
        return length * (1.0 - Shortened);
}

std::vector<double> WPStaccatissimo::modifyAmp(double time, std::vector<double> amp)
{
    if (time < length * Shortened)
    {
        return amp * std::exp(time / (length * Shortened));
    }
    return 0;
}
