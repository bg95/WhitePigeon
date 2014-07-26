#include "WP12EqualTuning.h"

#include <cstring>

void *(*WP12EqualTuning::callback)(WPCallbackManager::CallbackFuncStatic) = 0;

WP12EqualTuning::WP12EqualTuning()
{
}

WP12EqualTuning::~WP12EqualTuning()
{
}

void WP12EqualTuning::reset()
{
    WPModifier::reset();
    basefreq = 440.0;
    //currentmultinoteiter = getNotes().begin();
    //qDebug("WP12EqualTuning::reset this = %X", (quint64)this);
}

void WP12EqualTuning::set(std::string para)
{
    std::istringstream iss(para + " ");
    if (!(iss >> basefreq))
        basefreq = 440.0;
}

std::vector<double> WP12EqualTuning::modifyFreq(double time, std::vector<double> freq)
{
    WPMultinote *cmnote = getCurrentMultinote();
    if (cmnote == 0)
    {
        qCritical("No current note found! this = %lX", (quint64)this);
    }
    std::vector<WPNote> notes = getCurrentMultinote()->getNotes();
    std::vector<WPNote>::iterator iter;
    freq.clear();
    for (iter = notes.begin(); iter != notes.end(); iter++)
        freq.push_back(cal12EqualTuning(*iter));
    return freq;
}

std::string WP12EqualTuning::getComment()
{
    return std::string("Twelve-tone equal temperament. The parameter is the base frequency (default 440.0Hz).");
}

double WP12EqualTuning::cal12EqualTuning(WPNote note)
{
    return basefreq * pow(2.0, (double)note.getPitch() / 12.0);
}
/*
extern "C"
{
    WPModifier *create()
    {
        return new WP12EqualTuning();
    }
    void destroy(WPModifier *p)
    {
        printf("destroy called\n");
        fflush(stdout);
        if (p)
            delete p;
        p = 0;
    }
    void setCallback(void *_callback)
    {
        WP12EqualTuning::callback = (void *(*)(WPCallbackManager::CallbackFunc))_callback;
    }
}
*/
