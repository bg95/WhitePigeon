#include "WPModifier.h"

WPModifier::WPModifier()
{
    wpmodifier_time = 0;
    wpmodifier_prevtime = 0;
}

void WPModifier::setTime(double t)
{
    wpmodifier_prevtime = wpmodifier_time;
    wpmodifier_time = t;
}

inline double WPModifier::getTime() const
{
    return wpmodifier_time;
}

inline double WPModifier::getPrevTime() const
{
    return wpmodifier_prevtime;
}

inline bool WPModifier::timePassed(double t)
{
    return t > wpmodifier_prevtime && t <= wpmodifier_time;
}

virtual void WPModifier::reset()
{
    wpmodifier_time = 0;
    wpmodifier_prevtime = 0;
}

virtual void WPModifier::setNotes(const std::vector<WPNote> &_notes)
{
    wpmodifier_notes = _notes;
}

inline void WPModifier::getNotes()
{
    return wpmodifier_notes;
}

virtual void WPModifier::set(std::string para)
{
}

virtual bool WPModifier::isGlobal()
{
    return false;
}

virtual bool WPModifier::isSingleNote()
{
    return true;
}

virtual bool WPModifier::isTuning()
{
    return false;
}

virtual bool WPModifier::isTimbreModifier()
{
    return false;
}

virtual bool WPModifier::isTempoModifier()
{
    return false;
}

virtual bool WPModifier::isNoteModifier()
{
    return false;
}

virtual bool WPModifier::isFreqModifier()
{
    return false;
}

virtual bool WPModifier::isAmpModifier()
{
    return false;
}

virtual std::string WPModifier::modifyTimbre()
{
    return "";
}

virtual double WPModifier::modifyTempo(double time, double tempo)
{
    return tempo;
}

virtual double WPModifier::modifyNote(double time)
{
    return timePassed(0.0) ? notes[0].getLength().toDouble() : -1.0;
}

virtual double WPModifier::modifyFreq(double time, double freq)
{
    return freq;
}

virtual double WPModifier::modifyAmp(double time, double amp)
{
    return amp;
}
