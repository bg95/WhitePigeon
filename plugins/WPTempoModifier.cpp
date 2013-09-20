#include "WPTempoModifier.h"


WPTempoModifier::WPTempoModifier()
{
    tempo = 100.0;
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
    WPModifierPlugin::reset();
}

void WPTempoModifier::set(std::string para)
{
    para.push_back(' ');
    if (sscanf(para.data(), "%lf", &tempo) == EOF)
    {
        tempo = 100.0;
    }
    printf("tempo set %s %lf\n", para.data(), tempo);
    fflush(stdout);
    modifyTempo(0, 0);
}

double WPTempoModifier::modifyTempo(double, double)
{
	printf("tempo return %lf\n", tempo);
	fflush(stdout);
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

