#ifndef WPTIMBRE_H
#define WPTIMBRE_H

#include "WPWave.h"

//#include "WPScore/WPLib.h"

class WPTimbre
{
public:
    const static double ControlRate = 480.0; //Hertz
    //const static WPTuningFork TuningFork;
    /*static double interpolate(double *f, double t)
    {
        int a;
        double p;
        a = quint32(t * ControlRate);
        p = t * ControlRate - a;
        return f[a] * (1.0 - p) + f[a + 1] * p;
    }*/
    static inline double interpolate(double t, double t0, double t1, double f0, double f1);

    WPTimbre();
    virtual ~WPTimbre();
    virtual void reset(); //called before synthesis of a note
    virtual void set(std::string para);
    virtual WPWave *synthesize(double dur, double time0, double time1, double amp0, double amp1, double freq0, double freq1) = 0;
};

//#include "WPTuningFork.h"

#endif // WPTIMBRE_H
