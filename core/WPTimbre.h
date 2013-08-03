#ifndef WPTIMBRE_H
#define WPTIMBRE_H

#include "WPWave.h"
#include "WPSynthesizer.h"

class WPTimbre
{
public:
    const static double ControlRate = 48.0; //Hertz
    //const static WPTuningFork TuningFork;

    WPTimbre();
    virtual WPWave *synthesize(double dur, double *amp, double *freq) = 0;
};

#include "WPTuningFork.h"

#endif // WPTIMBRE_H
