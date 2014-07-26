#ifndef WPDEFAULTNOTEMODIFIER_H
#define WPDEFAULTNOTEMODIFIER_H

#include "core/WPModifier.h"
#include "WPScore/WPLib.h"
#include "WPScore/WPNote.h"
#include "WPScore/WPMultinote.h"
#include "WPDLLManager/WPCallbackManager.h"

class WPSlur : public WPModifier
{
public:
    WPSlur();
    ~WPSlur();
    void setNotes(WPMultinote *notes, int num, double offset);
    bool isNoteModifier()
    {
        return true;
    }
    double modifyNote(double time);
    
private:
	double sumlength;
    
};

#endif
