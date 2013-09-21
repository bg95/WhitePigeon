#ifndef WPCALLBACKMANAGER_H
#define WPCALLBACKMANAGER_H

class WPCallbackManager;
typedef void (WPCallbackManager::*MembFuncPtr)();

class WPCallbackManager
{
public:
    enum CallbackFuncStatic
    {
        WPWave_defaultAudioFormat,
        WPWave_newWPWave,
        WPSynthesizer_truncateWaveData
    };
    enum CallbackFuncMember
    {
        WPWave_mixWith,
        WPWave_append,
        WPWave_copy,

        WPNote_getLength,

        WPMultinote_getLength,
		//WPMultinote_getLengthDouble,

        Fraction_toDouble
    };
    static void *callStatic(CallbackFuncStatic func);
    static MembFuncPtr callMember(CallbackFuncMember func);

private:
    static void *CallbackFuncListStatic[];
    static void (WPCallbackManager::*CallbackFuncListMember[])();

};

#endif // WPCALLBACKMANAGER_H
