#ifndef WPMODIFIER_H
#define WPMODIFIER_H

#include "WPScore/WPNote.h"
#include "WPScore/WPMultinote.h"

class WPModifier
{
public:
    WPModifier();
    virtual ~WPModifier();
    virtual void setTime(double t);
    //this will be called before each call of modify* function, telling the current time (in beats)
    //call this when overwriting
    double getTime() const;
    double getPrevTime() const;
    bool timePassed(double t);
    virtual void reset(); //call this when overwriting
    //virtual void reset(double t); //call this when overwriting

    virtual void setNotes(const std::vector<WPMultinote> &notes, Fraction offset);
    //setNotes will be called before calling all modify* functions, telling the notes for the modifier
    //call WPModifier::setNotes when overwriting, or getNotes won't work
    std::vector<WPMultinote> &getNotes(); //why can't be inline?
    //virtual void setNote(const WPNote &_note); //similar to setNotes, but for isSingleNote() == true
    //WPNote &getNote();
    Fraction getNotesOffset();

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

private:
    double wpmodifier_time, wpmodifier_prevtime;
    std::vector<WPMultinote> wpmodifier_notes;
    Fraction wpmodifier_notesoffset;
    //WPNote wpmodifier_note;

};

#endif // WPMODIFIER_H
