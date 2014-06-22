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
    currentmultinoteiter = getNotes();
}

void WPModifier::setNotes(WPMultinote *notes, int num, double offset)
{
    wpmodifier_notes = notes;
    wpmodifier_notesnumber = num;
    wpmodifier_notesoffset = offset;
    //qDebug("WPModifier::setNotes this = %X wpmodifier_notes.begin() = %X", (quint64)this, (quint64)&*wpmodifier_notes.begin());
}

WPMultinote *WPModifier::getNotes()
{
    //qDebug("WPModifier::getNotes this = %X wpmodifier_notes.begin() = %X", (quint64)this, (quint64)&*wpmodifier_notes.begin());
    return wpmodifier_notes;
}

int WPModifier::getNotesNumber()
{
    return wpmodifier_notesnumber;
}

double WPModifier::getNotesOffset()
{
    return wpmodifier_notesoffset;
}

void WPModifier::set(std::string para)
{
}

bool WPModifier::isGlobal()
{
    return false;
}

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
    return timePassed(0.0);// ? wpmodifier_notes[0].getLength().toDouble() : -1.0;
}

std::vector<double> WPModifier::modifyFreq(double time, std::vector<double> freq)
{
    return freq;
}

std::vector<double> WPModifier::modifyAmp(double time, std::vector<double> amp)
{
    return amp;
}

WPMultinote *WPModifier::getCurrentMultinote()
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

