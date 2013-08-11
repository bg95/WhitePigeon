#ifndef WPTUNINGFORK_H
#define WPTUNINGFORK_H

#include "WPTimbre.h"
#include "WPSynthesizer.h"

class WPTuningFork : public WPTimbre
{
public:
    //const static WPTuningFork TuningFork;

    WPTuningFork();
    WPWave *synthesize(double dur, double *amp, double *freq) const;
};

#endif // WPTUNINGFORK_H
