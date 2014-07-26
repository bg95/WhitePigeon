#ifndef WPAMPMODIFIER_H
#define WPAMPMODIFIER_H

#include "core/WPModifier.h"
#include "WPDLLManager/WPCallbackManager.h"

class WPAmpModifier : public WPModifier
{
public:
    WPAmpModifier();
    ~WPAmpModifier();

    Precedence isAmpModifier();
    void set(std::string para);
    std::vector<double> modifyAmp(double time, std::vector<double> amp);

private:
    double amp;
};

#endif // WPAMPMODIFIER_H
