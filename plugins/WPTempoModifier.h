#ifndef WPTEMPOMODIFIER_H
#define WPTEMPOMODIFIER_H

//#include "core/WPModifier.h"
#include "WPModifierPlugin.h"

class WPTempoModifier : public WPModifierPlugin
{
public:
    WPTempoModifier();
    ~WPTempoModifier();

    bool isTempoModifier();
    void reset();
    void set(std::string para);
    double modifyTempo(double, double);

private:
    double tempo;
};

#endif // WPTEMPOMODIFIER_H
