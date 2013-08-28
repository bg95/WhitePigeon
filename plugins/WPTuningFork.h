#ifndef WPTUNINGFORK_H
#define WPTUNINGFORK_H

#include "core/WPTimbre.h"
#include "core/WPSynthesizer.h"
#include "WPDLLManager/WPCallbackManager.h"

class WPTuningFork : public WPTimbre
{
public:
    //const static WPTuningFork TuningFork;

    WPTuningFork();
    WPTuningFork(int);
    WPTuningFork(double);
    ~WPTuningFork();
    void reset();
    WPWave *synthesize(double dur, double *amp, double *freq) const;
    WPWave *synthesize(double dur, double time0, double time1, double amp0, double amp1, double freq0, double freq1);
    
    static void *(*callback)(WPCallbackManager::CallbackFunc);
    
private:
	double time, phi;

};

#endif // WPTUNINGFORK_H
