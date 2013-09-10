#include "WP12EqualTuning.h"

#include <cstring>

void *(*WP12EqualTuning::callback)(WPCallbackManager::CallbackFunc) = 0;

WP12EqualTuning::WP12EqualTuning()
{
}

WP12EqualTuning::~WP12EqualTuning()
{
}

void WP12EqualTuning::reset()
{
    basefreq = 440.0;
    currentmultinoteiter = getNotes().begin();
    qDebug("WP12EqualTuning::reset this = %X", (quint64)this);
}

void WP12EqualTuning::set(std::string para)
{
    std::istringstream iss(para + " ");
    if (!(iss >> basefreq))
        basefreq = 440.0;
}

std::vector<double> WP12EqualTuning::modifyFreq(double time, std::vector<double> freq)
{
    //WPMultinote cmnote = getCurrentMultinote();
    std::vector<WPNote> notes = getCurrentMultinote().getNotes();
    std::vector<WPNote>::iterator iter;
    freq.clear();
    for (iter = notes.begin(); iter != notes.end(); iter++)
        freq.push_back(cal12EqualTuning(*iter));
    return freq;
}

WPMultinote WP12EqualTuning::getCurrentMultinote()
{
    double time = getTime();
    double stime = (-getNotesOffset()).toDouble();
    std::vector<WPMultinote> &notes = getNotes();
    bool frombegin = false;
    qDebug("WP12EqualTuning::getCurrentMultinote this = %X notes.begin = %X", (quint64)this, (quint64)&*(notes.begin()));
    currentmultinoteiter = notes.begin();
    while (!frombegin)
    {
        if (currentmultinoteiter == notes.end())
            currentmultinoteiter = notes.begin();
        if (currentmultinoteiter == notes.begin())
            frombegin = true;
        for (; currentmultinoteiter != notes.end(); currentmultinoteiter++)
        {
            if (time >= stime && time < stime + (*currentmultinoteiter).getLength().toDouble())
                return (*currentmultinoteiter);
            stime += (*currentmultinoteiter).getLength().toDouble();
        }
    }
    return WPMultinote();
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
