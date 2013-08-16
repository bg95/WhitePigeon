#ifndef WPTUNINGFORK_H
#define WPTUNINGFORK_H

#include "WPTimbre.h"
#include "WPSynthesizer.h"
#include "../WPDLLManager/WPCallbackManager.h"

class WPTuningFork : public WPTimbre
{
public:
    //const static WPTuningFork TuningFork;

    WPTuningFork();
    WPTuningFork(int);
    WPTuningFork(double);
    ~WPTuningFork();
    WPWave *synthesize(double dur, double *amp, double *freq) const;
    
    static void *(*callback)(WPCallbackManager::CallbackFunc);
};

#endif // WPTUNINGFORK_H
