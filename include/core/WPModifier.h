#ifndef WPMODIFIER_H
#define WPMODIFIER_H

#include "WPScore/WPNote.h"
#include "WPScore/WPMultinote.h"
#include "WPScore/WPLib.h"
#include "core/WPWave.h"

class WPModifier
{
public:
    //A type for describing this modifier requires the information of how many notes.
    enum NotesRequirement
    {
        NONE,   //No notes required.
        SINGLE, //Require a single note. Used if the modifier applies to single notes, or application between notes does not interrelate
        RANGE   //Require all notes in the range of this modifier.
    };

    //A type for the precedence of modifiers. Returned by is*Modifier
    enum Precedence
    {
        NEVER = 0,  //This modifier does not modify the specific property.
        INIT = 1,   //Initializing modifier. Highest precedence. No more than one can exist simultaneously. Eg. tuning, fixed tempo.
        DISPROP = 2,//Disproportional modifier. Middle precedence. Not transitive. No more than one can exist simultaneously.
        PROP = 3    //Proportional modifier. Lowest precedence. Transitive. Multiple ones can exists simultanesouly. Scale the value by some factor. Only for tempo, frequency, amplitude.
    };

    WPModifier();
    virtual ~WPModifier();

    //setTime will be called before each call of modify* function, telling the current time (in beats)
    //call this when overwriting
    virtual void setTime(double t);
    virtual double getTime() const; //should be protected
    virtual double getPrevTime() const; //should be protected
    virtual bool timePassed(double t); //should be protected
    virtual void reset(); //call this when overwriting. Deprecated.
    //virtual void reset(double t); //call this when overwriting

    //This indicates whether the modifier needs the information of the notes.
    //Possible values may be NONE, SINGLE(single note), RANGE(all notes in the range).
    //If it is NONE, setNote() will always pass meaningless values
    //If it is SINGLE, setNote() will always pass a single note
    //If it is RANGE, setNote() will always pass all nodes in the range of this modifier
    virtual NotesRequirement needNotes();

    //setNotes will be called before calling all modify* functions, telling the notes for the modifier
    //call WPModifier::setNotes when overwriting, or getNotes won't work
    //virtual void setNotes(const std::vector<WPMultinote> &notes, double offset);
    virtual void setNotes(WPMultinote *notes, int num, double offset); //want to change to std::vector
    //virtual std::vector<WPMultinote> &getNotes(); //why can't be inline?
    virtual WPMultinote *getNotes(); //want to change to std::vector
    virtual int getNotesNumber();
    virtual double getNotesOffset();

    virtual bool isGlobal(); //true if the modifier applies to all parts. Unused now.

    virtual bool isTuning(); //if true, isFreqModifier is overridden to be INIT.
    virtual bool isTimbreModifier();//TODO: should be removed later
    virtual bool isTimbre();
    virtual Precedence isTempoModifier();
    virtual bool isNoteModifier();
    virtual Precedence isFreqModifier();
    virtual Precedence isAmpModifier();

    virtual void set(std::string para); //set parameters

    //time is in the unit of beats
    //length is the total duration of the property
    virtual std::string modifyTimbre(); //deprecated
    virtual WPWave *synthesize(double dur, double time0, double time1, double amp0, double amp1, double freq0, double freq1); //TODO: not in lib
    virtual double modifyTempo(double time, double tempo);
    virtual double modifyNote(double time); //if a note starts at (near) time, returns the length, otherwise returns a negative
    virtual std::vector<double> modifyFreq(double time, std::vector<double> freq);
    virtual std::vector<double> modifyAmp(double time, std::vector<double> amp);

    virtual WPMultinote *getCurrentMultinote();

    virtual std::string getComment(); //Returns a human-readable description of the modifier

private:
    double wpmodifier_time, wpmodifier_prevtime, wpmodifier_synthesized_time;
    //std::vector<WPMultinote> wpmodifier_notes;
    WPMultinote *wpmodifier_notes;
    int wpmodifier_notesnumber;
    double wpmodifier_notesoffset;
    //WPNote wpmodifier_note;
    WPMultinote *currentmultinoteiter;
    double wpmodifier_stime;

};


#endif // WPMODIFIER_H
