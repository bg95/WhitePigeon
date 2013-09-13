#include "WPAmpModifier.h"

void *(*WPAmpModifier::callback)(WPCallbackManager::CallbackFunc) = 0;


WPAmpModifier::WPAmpModifier()
{
}

WPAmpModifier::~WPAmpModifier()
{
}

bool WPAmpModifier::isAmpModifier()
{
    return true;
}

void WPAmpModifier::reset()
{
    amp = 1.0;
}

void WPAmpModifier::set(std::string para)
{
    para.push_back(' ');
    if (sscanf(para.data(), "%lf", &amp) == EOF)
    {
        amp = 1.0;
    }
}

double WPAmpModifier::modifyAmp(double, double)
{
    return amp;
}

/*
extern "C"
{
    WPModifier *create()
    {
        return new WPDefaultNoteModifier();
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
        WPDefaultNoteModifier::callback = (void *(*)(WPCallbackManager::CallbackFunc))_callback;
    }
}
*/
