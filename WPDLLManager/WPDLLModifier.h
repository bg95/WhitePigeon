#ifndef WPDLLMODIFIER_H
#define WPDLLMODIFIER_H

#include "WPDLLManager.h"
#include "core/WPModifier.h"

class WPDLLModifier : public WPModifier
{
public:
    WPDLLModifier();
    ~WPDLLModifier();
    bool loadDLL(std::string name);

    void reset();

    void setNotes(const std::vector<WPMultinote> &_notes, Fraction offset);
    //setNotes will be called before calling all modify* functions, telling the notes for the modifier
    //call WPModifier::setNotes when overwriting, or getNotes won't work
    void setNote(const WPNote &_note); //similar to setNotes, but for isSingleNote() == true

    bool isGlobal(); //true if the modifier applies to all parts
    bool isSingleNote();
    //true if the modifier applies to single notes, or application between notes does not interrelate
    //if true, setNotes() will always pass a single note, isGlobal must be false

    bool isTuning(); //if true, isFreqModifier must be true
    bool isTimbreModifier();
    bool isTempoModifier();
    bool isNoteModifier();
    bool isFreqModifier();
    bool isAmpModifier();

    void set(std::string para); //set parameters

    //time is in the unit of beats
    //length is the total duration of the property
    std::string modifyTimbre();
    double modifyTempo(double time, double tempo);
    double modifyNote(double time); //if a note starts at (near) time, returns the length, otherwise returns a negative
    std::vector<double> modifyFreq(double time, std::vector<double> freq);
    std::vector<double> modifyAmp(double time, std::vector<double> amp);


private:
    WPModifier *modifier;
    WPDLLManager *manager;

};

#endif // WPDLLMODIFIER_H
