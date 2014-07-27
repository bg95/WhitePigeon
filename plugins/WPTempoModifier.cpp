#include "WPTempoModifier.h"


WPTempoModifier::WPTempoModifier()
{
    tempo = 100.0;
}

WPTempoModifier::~WPTempoModifier()
{
}

WPModifier::Precedence WPTempoModifier::isTempoModifier()
{
    return INIT;
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
    }
}

