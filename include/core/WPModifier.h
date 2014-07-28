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

    //Access absolute start time of the property, which is
    // The start of the property - if needNotes is NONE or RANGE
    // The start of current note - if needNotes is SINGLE
    void setAbsoluteStartTime(double t);
    double getAbsoluteStartTime() const;
    //setTime will be called before each call of modify* function, telling the current time (in beats)
    //The time is relative to getAbsoluteStartTime()
    //Overwriting is not recommended. Call this when overwriting.
    //getTime is protected (maybe it need not to be)
    virtual void setTime(double t);

    //This indicates whether the modifier needs the information of the notes.
    //Possible values may be NONE, SINGLE (single note), RANGE (all notes in the range).
    //If it is NONE, setNotes() will always pass meaningless values. setNotes() will be called once.
    //If it is SINGLE, setNotes() will always pass a single note. setNotes() will be called every time a new multinote is going to be processed.
    //If it is RANGE, setNotes() will always pass all nodes in the range of this modifier. setNotes() will be called once.
    virtual NotesRequirement needNotes();

    //setNotes will be called before calling all modify* functions, telling the notes for the modifier
    //call WPModifier::setNotes when overwriting, or getNotes won't work
    //virtual void setNotes(const std::vector<WPMultinote> &notes, double offset);
    //virtual void setNotes(WPMultinote *notes, int num, double offset); //want to change to std::vector
    void setNotes(std::vector<WPMultinote> notes, double offset); //TODO: not virtual. calls reset
    //virtual std::vector<WPMultinote> &getNotes(); //why can't be inline?
    //virtual WPMultinote *getNotes(); //want to change to std::vector
    std::vector<WPMultinote> &getNotes(); //TODO
    int getNotesNumber();
    double getNotesOffset();

    virtual bool isGlobal(); //true if the modifier applies to all parts. Unused now. May be supported by the editor.

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

    virtual std::string getComment(); //Returns a human-readable description of the modifier

protected:
    virtual double getTime() const; //should be protected
    virtual double getPrevTime() const; //should be protected
    virtual bool timePassed(double t); //should be protected
    virtual void reset(); //called by setNotes. should be protected
    //virtual void reset(std::vector<WPMultinote> notes, double offset); //TODO: want to make setNotes call this
    //virtual void reset(double t); //call this when overwriting

    virtual WPMultinote *getCurrentMultinote(); //should be protected

private:
    double wpmodifier_absstarttime;
    double wpmodifier_time, wpmodifier_prevtime, wpmodifier_synthesized_time;
    //std::vector<WPMultinote> wpmodifier_notes;
    //WPMultinote *wpmodifier_notes;
    std::vector<WPMultinote> wpmodifier_notes;
    int wpmodifier_notesnumber;
    double wpmodifier_notesoffset;
    //WPNote wpmodifier_note;
    WPMultinote *currentmultinoteiter; //used by getCurrentMultinote
    double wpmodifier_stime;

};


#endif // WPMODIFIER_H
