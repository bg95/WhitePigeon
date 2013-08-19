#ifndef WPCALLBACKMANAGER_H
#define WPCALLBACKMANAGER_H

#include "core/WPWave.h"
#include "core/WPSynthesizer.h"

class WPCallbackManager
{
public:
    enum CallbackFunc
    {
        WPWave_defaultAudioFormat,
        WPWave_newWPWave,
        WPSynthesizer_truncateWaveData
    };
    static void *call(CallbackFunc func);

private:
    static void *CallbackFuncList[];

};

#endif // WPCALLBACKMANAGER_H
