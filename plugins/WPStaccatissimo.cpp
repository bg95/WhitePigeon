#include "WPStaccatissimo.h"

WPStaccatissimo::WPStaccatissimo()
{
}

void WPStaccatissimo::setNotes(const std::vector<WPNote> &_notes)
{
    std::vector<WPNote>::iterator iter;
    WPModifier::setNotes(_notes);
    length = 0;
    for (iter = getNotes().begin(); iter != getNotes().end(); iter++)
        length += (*iter).getLength().toDouble();
}

double WPStaccatissimo::modifyNote(double time)
{
    if (timePassed(0.0))
        return length * Shortened;
    if (timePassed(length * Shortened))
        return length * (1.0 - Shortened);
}

double WPStaccatissimo::modifyAmp(double time, double amp)
{
    if (time < length * Shortened)
    {
        return std::exp(time / (length * Shortened));
    }
    return 0;
}
