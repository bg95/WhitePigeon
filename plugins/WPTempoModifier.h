#ifndef WPTEMPOMODIFIER_H
#define WPTEMPOMODIFIER_H

#include "WPModifier.h"

class WPTempoModifier : public WPModifier
{
public:
    WPTempoModifier();
    ~WPTempoModifier();

    Precedence isTempoModifier();
    void set(std::string para);
    double modifyTempo(double, double);

private:
    double tempo;
};

#endif // WPTEMPOMODIFIER_H
