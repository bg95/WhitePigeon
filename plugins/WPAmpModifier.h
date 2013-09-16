#ifndef WPAMPMODIFIER_H
#define WPAMPMODIFIER_H

#include "core/WPModifier.h"
#include "WPDLLManager/WPCallbackManager.h"

class WPAmpModifier : public WPModifier
{
public:
    WPAmpModifier();
    ~WPAmpModifier();

    bool isAmpModifier();
    void reset();
    void set(std::string para);
    double modifyAmp(double, double);

    static void *(*callback)(typename WPCallbackManager::CallbackFunc);

private:
    double amp;
};

#endif // WPAMPMODIFIER_H
