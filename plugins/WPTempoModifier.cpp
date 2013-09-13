#include "WPTempoModifier.h"

void *(*WPTempoModifier::callback)(WPCallbackManager::CallbackFunc) = 0;


WPTempoModifier::WPTempoModifier()
{
}

WPTempoModifier::~WPTempoModifier()
{
}

bool WPTempoModifier::isTempoModifier()
{
    return true;
}

void WPTempoModifier::reset()
{
    tempo = 100.0;
}

void WPTempoModifier::set(std::string para)
{
    para.push_back(' ');
    if (sscanf(para.data(), "%lf", &tempo) == EOF)
    {
        tempo = 100.0;
    }
}

double WPTempoModifier::modifyTempo(double, double)
{
    return tempo;
}

extern "C"
{
    WPModifier *create()
    {
        return new WPTempoModifier();
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
        WPTempoModifier::callback = (void *(*)(WPCallbackManager::CallbackFunc))_callback;
    }
}

