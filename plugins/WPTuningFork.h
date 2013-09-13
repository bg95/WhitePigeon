#ifndef WPTUNINGFORK_H
#define WPTUNINGFORK_H

#include "core/WPTimbre.h"
#include "core/WPTimbre.cpp"
#include "core/WPSynthesizer.h"
#include "WPDLLManager/WPCallbackManager.h"

#include "WPScore/WPLib.h"
#include "WPScore/WPLib.cpp"

#include <cstdlib>

class WPTuningFork : public WPTimbre
{
public:
    //const static WPTuningFork TuningFork;

    WPTuningFork();
    ~WPTuningFork();
    void reset();
    //WPWave *synthesize(double dur, double *amp, double *freq) const;
    WPWave *synthesize(double dur, double time0, double time1, double amp0, double amp1, double freq0, double freq1);
    //void set2(Fraction g);
    
    static void *(*callback)(WPCallbackManager::CallbackFunc);
    
private:
	double time, phi, phi2;
	double envelope(double rise, double sustain, double decay, double t);
	double rand11();

};

#endif // WPTUNINGFORK_H
