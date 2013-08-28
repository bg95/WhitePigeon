#ifndef WPDEFAULTNOTEMODIFIER_H
#define WPDEFAULTNOTEMODIFIER_H

#include "core/WPModifier.h"
#include "WPScore/WPNote.h"
#include "WPScore/WPMultiNote.h"
#include "WPDLLManager/WPCallbackManager.h"

class WPDefaultNoteModifier : public WPModifier
{
public:
    void setNotes(const std::vector<WPMultinote> &notes, Fraction offset);
    bool isNoteModifier()
    {
        return true;
    }
    double modifyNote(double time);
    
    static void *(*callback)(WPCallbackManager::CallbackFunc);
};

#endif
