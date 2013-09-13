#ifndef WPTEMPOMODIFIER_H
#define WPTEMPOMODIFIER_H

#include "core/WPModifier.h"
#include "WPDLLManager/WPCallbackManager.h"

class WPTempoModifier : public WPModifier
{
public:
    WPTempoModifier();
    ~WPTempoModifier();

    bool isTempoModifier();
    void reset();
    void set(std::string para);
    double modifyTempo(double, double);

    static void *(*callback)(WPCallbackManager::CallbackFunc);

private:
    double tempo;
};

#endif // WPTEMPOMODIFIER_H
