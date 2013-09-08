#include "WP12EqualTuning.h"

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
}

void WP12EqualTuning::set(std::string para)
{
    std::istringstream iss(para + " ");
    if (!(iss >> basefreq))
        basefreq = 440.0;
}

std::vector<double> WP12EqualTuning::modifyFreq(double time, std::vector<double> freq)
{
    WPMultinote cmnote = getCurrentMultinote();
    std::vector<WPNote>::iterator iter;
    freq.clear();
    for (iter = cmnote.getNotes().begin(); iter != cmnote.getNotes().end(); iter++)
        freq.push_back(cal12EqualTuning(*iter));
    return freq;
}

WPMultinote WP12EqualTuning::getCurrentMultinote()
{
    double time = getTime();
    double stime = (-getNotesOffset()).toDouble();
    std::vector<WPMultinote> notes = getNotes();
    std::vector<WPMultinote>::iterator iter;
    for (iter = notes.begin(); iter != notes.end(); iter++)
    {
        if (time >= stime && time < stime + (*iter).getLength().toDouble())
            return (*iter);
        stime += (*iter).getLength().toDouble();
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
