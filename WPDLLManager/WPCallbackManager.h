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
    static void init();
    static void *call(CallbackFunc func);

private:
    static const int CallbackFuncNum;
    static void *CallbackFuncList[];
    static bool initialized;

};

#endif // WPCALLBACKMANAGER_H
