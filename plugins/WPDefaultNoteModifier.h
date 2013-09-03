#ifndef WPDEFAULTNOTEMODIFIER_H
#define WPDEFAULTNOTEMODIFIER_H

#include "core/WPModifier.h"
#include "WPScore/WPNote.h"
#include "WPScore/WPMultinote.h"
class WPDefaultNoteModifier;
#include "WPDLLManager/WPCallbackManager.h"

class WPDefaultNoteModifier : public WPModifier
{
public:
    WPDefaultNoteModifier();
    ~WPDefaultNoteModifier();
    void setNotes(const std::vector<WPMultinote> &notes, Fraction offset);
    bool isNoteModifier()
    {
        return true;
    }
    double modifyNote(double time);

    static void *(*callback)(typename WPCallbackManager::CallbackFunc);
    
private:
    std::vector<double> notestarts;
    
};

#endif
