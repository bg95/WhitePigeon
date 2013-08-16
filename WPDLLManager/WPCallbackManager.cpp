#include "WPCallbackManager.h"

const int WPCallbackManager::CallbackFuncNum = 3;
void *WPCallbackManager::CallbackFuncList[WPCallbackManager::CallbackFuncNum];
bool WPCallbackManager::initialized = false;

void WPCallbackManager::init()
{
    if (initialized)
        return;
    qDebug("Callback functions: %d", CallbackFuncNum);
    CallbackFuncList[WPWave_defaultAudioFormat] = (void *)WPWave::defaultAudioFormat;
    CallbackFuncList[WPWave_newWPWave] = (void *)WPWave::newWPWave;
    CallbackFuncList[WPSynthesizer_truncateWaveData] = (void *)WPSynthesizer::truncateWaveData;
    initialized = true;
}

void *WPCallbackManager::call(CallbackFunc func)
{
    if (!initialized)
    {
        qCritical("WPCallbackManager not initialized!");
        return 0;
    }
    return CallbackFuncList[func];
}

