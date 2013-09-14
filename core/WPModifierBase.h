#ifndef WPMODIFIERBASE_H
#define WPMODIFIERBASE_H

#include "WPScore/WPMultinote.h"
#include "WPScore/WPLib.h"

class WPModifierBase
{
public:
    WPModifierBase() {}
    virtual ~WPModifierBase() {}
    virtual void setTime(double t) = 0;
    //this will be called before each call of modify* function, telling the current time (in beats)
    //call this when overwriting
    virtual double getTime() const = 0;
    virtual double getPrevTime() const = 0;
    virtual bool timePassed(double t) = 0;
    virtual void reset() = 0; //call this when overwriting

    virtual void setNotes(const std::vector<WPMultinote> &notes, Fraction offset) = 0;
    //setNotes will be called before calling all modify* functions, telling the notes for the modifier
    //call WPModifier::setNotes when overwriting, or getNotes won't work
    virtual std::vector<WPMultinote> &getNotes() = 0; //why can't be inline?
    virtual Fraction getNotesOffset() = 0;

    virtual bool isGlobal() = 0; //true if the modifier applies to all parts

    virtual bool isTuning() = 0; //if true, isFreqModifier must be true
    virtual bool isTimbreModifier() = 0;
    virtual bool isTempoModifier() = 0;
    virtual bool isNoteModifier() = 0;
    virtual bool isFreqModifier() = 0;
    virtual bool isAmpModifier() = 0;

    virtual void set(std::string para) = 0; //set parameters

    //time is in the unit of beats
    //length is the total duration of the property
    virtual std::string modifyTimbre() = 0;
    virtual double modifyTempo(double time, double tempo) = 0;
    virtual double modifyNote(double time) = 0; //if a note starts at (near) time, returns the length, otherwise returns a negative
    virtual std::vector<double> modifyFreq(double time, std::vector<double> freq) = 0;
    virtual std::vector<double> modifyAmp(double time, std::vector<double> amp) = 0;
};

#endif // WPMODIFIERBASE_H
