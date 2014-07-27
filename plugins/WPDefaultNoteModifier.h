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
    NotesRequirement needNotes()
    {
        return RANGE;
    }
    //void setNotes(const std::vector<WPMultinote> &notes, double offset);
    //void setNotes(WPMultinote *notes, int num, double offset);
    void reset();
    bool isNoteModifier()
    {
        return true;
    }
    double modifyNote(double time);

    std::string getComment();

    //static void *(*callback)(typename WPCallbackManager::CallbackFuncStatic);
    
private:
    std::vector<double> notestarts;
    int notei;
    
};

#endif
