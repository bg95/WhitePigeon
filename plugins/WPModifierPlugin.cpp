#include "WPModifierPlugin.h"

WPModifierPlugin::WPModifierPlugin()
{
    reset();
}

WPModifierPlugin::~WPModifierPlugin()
{
}

void WPModifierPlugin::setTime(double t)
{
    wpmodifier_prevtime = wpmodifier_time;
    wpmodifier_time = t;
}

double WPModifierPlugin::getTime() const
{
    return wpmodifier_time;
}

double WPModifierPlugin::getPrevTime() const
{
    return wpmodifier_prevtime;
}

inline bool WPModifierPlugin::timePassed(double t)
{
    return t > wpmodifier_prevtime && t <= wpmodifier_time;
}

void WPModifierPlugin::reset()
{
    wpmodifier_time = 0.0;
    wpmodifier_prevtime = 0.0;
    currentmultinoteiter = getNotes();
}

void WPModifierPlugin::setNotes(WPMultinote *notes, int num, double offset)
{
    wpmodifier_notes = notes;
    wpmodifier_notesnumber = num;
    wpmodifier_notesoffset = offset;
    //qDebug("WPModifierPlugin::setNotes this = %X wpmodifier_notes.begin() = %X", (quint64)this, (quint64)&*wpmodifier_notes.begin());
}

WPMultinote *WPModifierPlugin::getNotes()
{
    //qDebug("WPModifierPlugin::getNotes this = %X wpmodifier_notes.begin() = %X", (quint64)this, (quint64)&*wpmodifier_notes.begin());
    return wpmodifier_notes;
}

int WPModifierPlugin::getNotesNumber()
{
    return wpmodifier_notesnumber;
}

double WPModifierPlugin::getNotesOffset()
{
    return wpmodifier_notesoffset;
}

void WPModifierPlugin::set(std::string para)
{
}

bool WPModifierPlugin::isGlobal()
{
    return false;
}

bool WPModifierPlugin::isTuning()
{
    return false;
}

bool WPModifierPlugin::isTimbreModifier()
{
    return false;
}

bool WPModifierPlugin::isTempoModifier()
{
    return false;
}

bool WPModifierPlugin::isNoteModifier()
{
    return false;
}

bool WPModifierPlugin::isFreqModifier()
{
    return false;
}

bool WPModifierPlugin::isAmpModifier()
{
    return false;
}

std::string WPModifierPlugin::modifyTimbre()
{
    return "";
}

double WPModifierPlugin::modifyTempo(double time, double tempo)
{
    return tempo;
}

double WPModifierPlugin::modifyNote(double time)
{
    return timePassed(0.0);// ? wpmodifier_notes[0].getLength().toDouble() : -1.0;
}

std::vector<double> WPModifierPlugin::modifyFreq(double time, std::vector<double> freq)
{
    return freq;
}

std::vector<double> WPModifierPlugin::modifyAmp(double time, std::vector<double> amp)
{
    return amp;
}

WPMultinote *WPModifierPlugin::getCurrentMultinote()
{
    double (WPMultinote::*getLengthDouble)() = (typeof getLengthDouble)callMember(WPCallbackManager::WPMultinote_getLengthDouble);
    
    double time = getTime();
    double dtime;
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
		    dtime = ((*currentmultinoteiter).*getLengthDouble)();
            if (time >= wpmodifier_stime && time < wpmodifier_stime + dtime)
            {
                //qDebug("WPModifier::getCurrentMultinote found %lX", (quint64)currentmultinoteiter);
                return currentmultinoteiter;
            }
            wpmodifier_stime += dtime;
        }
    }
    return notes + n - 1;
}


void *(*WPModifierPlugin::callStatic)(WPCallbackManager::CallbackFuncStatic) = 0;
MembFuncPtr (*WPModifierPlugin::callMember)(WPCallbackManager::CallbackFuncMember) = 0;
	
extern "C"
{
	void setCallbackStatic(void *(*callStatic)(WPCallbackManager::CallbackFuncStatic))
	{
		WPModifierPlugin::callStatic = callStatic;
	}
	void setCallbackMember(MembFuncPtr (*callMember)(WPCallbackManager::CallbackFuncMember))
	{
		WPModifierPlugin::callMember = callMember;
	}
}

