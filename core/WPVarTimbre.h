#ifndef WPVARTIMBRE_H
#define WPVARTIMBRE_H

#include <QString>
#include "WPTimbre.h"

class WPVarTimbre : public WPTimbre
{
public:
    WPVarTimbre()
    {}
    virtual void set(QString str) = 0;
};

#endif // WPVARTIMBRE_H

/*
 * other classes:
 * WPVowel, WPConsonantVowel : WPTimbre
 * WPVoice : WPVarTimbre
 * WPVoiceAnalyzer
 * WPBranchedPipe : QIODevice
 * WPEffect: WPNoteModifier, WPFreqModifier, WPDurModifier, WPAmpModifier, WPTimbre
 * WPPropertyApplicator
 */
