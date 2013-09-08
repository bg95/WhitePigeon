#include "core/WPWave.h"
#include "core/WPSynthesizer.h"
#include "WPCallbackManager.h"

void *WPCallbackManager::CallbackFuncList[] =
{
    (void *)WPWave::defaultAudioFormat,
    (void *)WPWave::newWPWave,
    (void *)WPSynthesizer::truncateWaveData
};

void *WPCallbackManager::call(CallbackFunc func)
{
    return CallbackFuncList[func];
}
