#include "core/WPWave.h"
#include "core/WPSynthesizer.h"
#include "WPScore/WPNote.h"
#include "WPScore/WPMultinote.h"
#include "WPScore/WPLib.h"

#include "WPCallbackManager.h"

void *WPCallbackManager::CallbackFuncListStatic[] =
{
    (void *)WPWave::defaultAudioFormat,
    (void *)WPWave::newWPWave,
    (void *)WPSynthesizer::truncateWaveData
};

void (WPCallbackManager::*WPCallbackManager::CallbackFuncListMember[])() =
{
        (MembFuncPtr)&WPWave::mixWith,
        (MembFuncPtr)&WPWave::append,
        (MembFuncPtr)&WPWave::copy,

        (MembFuncPtr)&WPNote::getLength,

        (MembFuncPtr)&WPMultinote::getLength,
		//(MembFuncPtr)&WPMultinote::getLengthDouble,

        (MembFuncPtr)&Fraction::toDouble
};

void *WPCallbackManager::callStatic(CallbackFuncStatic func)
{
    return CallbackFuncListStatic[func];
}

MembFuncPtr WPCallbackManager::callMember(CallbackFuncMember func)
{
    return CallbackFuncListMember[func];
}
