#ifndef WP12EQUALTUNING_H
#define WP12EQUALTUNING_H

#include <Qt>
#include <sstream>
#include "core/WPModifier.h"
#include "WPScore/WPNote.h"
#include "WPScore/WPMultinote.h"
class WP12EqualTuning;
#include "WPDLLManager/WPCallbackManager.h"

class WP12EqualTuning : public WPModifier
{
public:
    WP12EqualTuning();
    ~WP12EqualTuning();
    void reset();
    bool isTuning()
    {
        return true;
    }

    void set(std::string para);

    std::vector<double> modifyFreq(double time, std::vector<double> freq);
    //WPMultinote *getCurrentMultinote();

    std::string getComment();

    //static void *(*callback)(WPCallbackManager::CallbackFuncStatic);
    
private:
    double cal12EqualTuning(WPNote note);
    double basefreq;
    //std::vector<WPMultinote>::iterator currentmultinoteiter;

};

#endif // WP12EQUALTUNING_H
