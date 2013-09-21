#include <Qt>
#include "WPModifierInternal.h"

WPModifierInternal::WPModifierInternal()
{
    reset();
}

WPModifierInternal::~WPModifierInternal()
{
}

void WPModifierInternal::setTime(double t)
{
    wpmodifier_prevtime = wpmodifier_time;
    wpmodifier_time = t;
}

double WPModifierInternal::getTime() const
{
    return wpmodifier_time;
}

double WPModifierInternal::getPrevTime() const
{
    return wpmodifier_prevtime;
}

inline bool WPModifierInternal::timePassed(double t)
{
    return t > wpmodifier_prevtime && t <= wpmodifier_time;
}

void WPModifierInternal::reset()
{
    wpmodifier_time = 0.0;
    wpmodifier_prevtime = 0.0;
    currentmultinoteiter = getNotes();
}

void WPModifierInternal::setNotes(WPMultinote *notes, int num, double offset)
{
    wpmodifier_notes = notes;
    wpmodifier_notesnumber = num;
    wpmodifier_notesoffset = offset;
    //qDebug("WPModifierInternal::setNotes this = %X wpmodifier_notes.begin() = %X", (quint64)this, (quint64)&*wpmodifier_notes.begin());
}

WPMultinote *WPModifierInternal::getNotes()
{
    //qDebug("WPModifierInternal::getNotes this = %X wpmodifier_notes.begin() = %X", (quint64)this, (quint64)&*wpmodifier_notes.begin());
    return wpmodifier_notes;
}

int WPModifierInternal::getNotesNumber()
{
    return wpmodifier_notesnumber;
}

double WPModifierInternal::getNotesOffset()
{
    return wpmodifier_notesoffset;
}

void WPModifierInternal::set(std::string para)
{
}

bool WPModifierInternal::isGlobal()
{
    return false;
}

bool WPModifierInternal::isTuning()
{
    return false;
}

bool WPModifierInternal::isTimbreModifier()
{
    return false;
}

bool WPModifierInternal::isTempoModifier()
{
    return false;
}

bool WPModifierInternal::isNoteModifier()
{
    return false;
}

bool WPModifierInternal::isFreqModifier()
{
    return false;
}

bool WPModifierInternal::isAmpModifier()
{
    return false;
}

std::string WPModifierInternal::modifyTimbre()
{
    return "";
}

double WPModifierInternal::modifyTempo(double time, double tempo)
{
    return tempo;
}

double WPModifierInternal::modifyNote(double time)
{
    return timePassed(0.0);// ? wpmodifier_notes[0].getLength().toDouble() : -1.0;
}

std::vector<double> WPModifierInternal::modifyFreq(double time, std::vector<double> freq)
{
    return freq;
}

std::vector<double> WPModifierInternal::modifyAmp(double time, std::vector<double> amp)
{
    return amp;
}

WPMultinote *WPModifierInternal::getCurrentMultinote()
{
    double time = getTime();
    //double stime = (-getNotesOffset())/*.toDouble()*/;//need to be modified to a member variable
    WPMultinote *notes = getNotes();
    int n = getNotesNumber();
    bool frombegin = false;
    //currentmultinoteiter = notes;
    //qDebug("WPModifier::getCurrentMultinote this = %lX begin at = %lX time = %lf", (quint64)this, (quint64)&*(currentmultinoteiter), time);
    while (!frombegin)
    {
        if (currentmultinoteiter == notes + n)
            currentmultinoteiter = notes;
        if (currentmultinoteiter == notes)
        {
            frombegin = true;
            wpmodifier_stime = (-getNotesOffset());
        }
        for (; currentmultinoteiter != notes + n; currentmultinoteiter++)
        {
            if (time >= wpmodifier_stime && time < wpmodifier_stime + (*currentmultinoteiter).getLength().toDouble())
            {
                //qDebug("WPModifier::getCurrentMultinote found %lX", (quint64)currentmultinoteiter);
                return currentmultinoteiter;
            }
            wpmodifier_stime += (*currentmultinoteiter).getLength().toDouble();
        }
    }
    return notes + n - 1;
}
