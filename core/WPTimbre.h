#ifndef WPTIMBRE_H
#define WPTIMBRE_H

#include "WPWave.h"

class WPTimbre
{
public:
    const static double ControlRate = 480.0; //Hertz
    //const static WPTuningFork TuningFork;
    static double interpolate(double *f, double t)
    {
        int a;
        double p;
        a = quint32(t * ControlRate);
        p = t * ControlRate - a;
        return f[a] * (1.0 - p) + f[a + 1] * p;
    }

    WPTimbre()
    {}
    virtual WPWave *synthesize(double dur, double *amp, double *freq) const = 0;
};

//#include "WPTuningFork.h"

#endif // WPTIMBRE_H
