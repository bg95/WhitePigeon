#ifndef WPMODIFIERPLUGIN_H
#define WPMODIFIERPLUGIN_H

#include "core/WPModifier.h"
#include "WPDLLManager/WPCallbackManager.h"
#include "WPScore/WPNote.h"
#include "WPScore/WPMultinote.h"
#include "WPScore/WPLib.h"

class WPModifierPlugin : public WPModifier
{
public:
    WPModifierPlugin();
    virtual ~WPModifierPlugin();
    virtual void setTime(double t);
    //this will be called before each call of modify* function, telling the current time (in beats)
    //call this when overwriting
    virtual double getTime() const;
    virtual double getPrevTime() const;
    virtual bool timePassed(double t);
    virtual void reset(); //call this when overwriting
    //virtual void reset(double t); //call this when overwriting

    //virtual void setNotes(const std::vector<WPMultinote> &notes, double offset);
    virtual void setNotes(WPMultinote *notes, int num, double offset);
    //setNotes will be called before calling all modify* functions, telling the notes for the modifier
    //call WPModifier::setNotes when overwriting, or getNotes won't work
    //virtual std::vector<WPMultinote> &getNotes(); //why can't be inline?
    //virtual double getNotesOffset();
    virtual WPMultinote *getNotes();
    virtual int getNotesNumber();
    virtual double getNotesOffset();

    virtual bool isGlobal(); //true if the modifier applies to all parts
    //virtual bool isSingleNote();
    //true if the modifier applies to single notes, or application between notes does not interrelate
    //if true, setNote() will always pass a single note, isGlobal must be false

    virtual bool isTuning(); //if true, isFreqModifier must be true
    virtual bool isTimbreModifier();
    virtual bool isTempoModifier();
    virtual bool isNoteModifier();
    virtual bool isFreqModifier();
    virtual bool isAmpModifier();

    virtual void set(std::string para); //set parameters

    //time is in the unit of beats
    //length is the total duration of the property
    virtual std::string modifyTimbre();
    virtual double modifyTempo(double time, double tempo);
    virtual double modifyNote(double time); //if a note starts at (near) time, returns the length, otherwise returns a negative
    virtual std::vector<double> modifyFreq(double time, std::vector<double> freq);
    virtual std::vector<double> modifyAmp(double time, std::vector<double> amp);

    virtual WPMultinote *getCurrentMultinote();

private:
    double wpmodifier_time, wpmodifier_prevtime;
    //std::vector<WPMultinote> wpmodifier_notes;
    WPMultinote *wpmodifier_notes;
    int wpmodifier_notesnumber;
    double wpmodifier_notesoffset;
    //WPNote wpmodifier_note;
    WPMultinote *currentmultinoteiter;
    double wpmodifier_stime;

public:
	static void *(*callStatic)(WPCallbackManager::CallbackFuncStatic);
	static MembFuncPtr (*callMember)(WPCallbackManager::CallbackFuncMember);

};

#endif // WPMODIFIERPLUGIN_H
