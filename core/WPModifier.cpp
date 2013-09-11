#include "WPModifier.h"

WPModifier::WPModifier()
{
    reset();
}

WPModifier::~WPModifier()
{
}

void WPModifier::setTime(double t)
{
    wpmodifier_prevtime = wpmodifier_time;
    wpmodifier_time = t;
}

double WPModifier::getTime() const
{
    return wpmodifier_time;
}

double WPModifier::getPrevTime() const
{
    return wpmodifier_prevtime;
}

inline bool WPModifier::timePassed(double t)
{
    return t > wpmodifier_prevtime && t <= wpmodifier_time;
}

void WPModifier::reset()
{
    wpmodifier_time = 0.0;
    wpmodifier_prevtime = 0.0;
}
/*
void WPModifier::reset(double t)
{
    wpmodifier_time = t;
    wpmodifier_prevtime = t;
    //wpmodifier_deltatime = 0.0;
}
*/
void WPModifier::setNotes(const std::vector<WPMultinote> &notes, Fraction offset)
{
    wpmodifier_notes = notes;
    wpmodifier_notesoffset = offset;
    //qDebug("WPModifier::setNotes this = %X wpmodifier_notes.begin() = %X", (quint64)this, (quint64)&*wpmodifier_notes.begin());
}

std::vector<WPMultinote> &WPModifier::getNotes()
{
    //qDebug("WPModifier::getNotes this = %X wpmodifier_notes.begin() = %X", (quint64)this, (quint64)&*wpmodifier_notes.begin());
    return wpmodifier_notes;
}

Fraction WPModifier::getNotesOffset()
{
    return wpmodifier_notesoffset;
}

/*
void WPModifier::setNote(const WPNote &_note)
{
    wpmodifier_note = _note;
}

WPNote &WPModifier::getNote()
{
    return wpmodifier_note;
}
*/
void WPModifier::set(std::string para)
{
}

bool WPModifier::isGlobal()
{
    return false;
}
/*
bool WPModifier::isSingleNote()
{
    return true;
}
*/
bool WPModifier::isTuning()
{
    return false;
}

bool WPModifier::isTimbreModifier()
{
    return false;
}

bool WPModifier::isTempoModifier()
{
    return false;
}

bool WPModifier::isNoteModifier()
{
    return false;
}

bool WPModifier::isFreqModifier()
{
    return false;
}

bool WPModifier::isAmpModifier()
{
    return false;
}

std::string WPModifier::modifyTimbre()
{
    return "";
}

double WPModifier::modifyTempo(double time, double tempo)
{
    return tempo;
}

double WPModifier::modifyNote(double time)
{
    return timePassed(0.0) ? wpmodifier_notes[0].getLength().toDouble() : -1.0;
}

std::vector<double> WPModifier::modifyFreq(double time, std::vector<double> freq)
{
    return freq;
}

std::vector<double> WPModifier::modifyAmp(double time, std::vector<double> amp)
{
    return amp;
}
