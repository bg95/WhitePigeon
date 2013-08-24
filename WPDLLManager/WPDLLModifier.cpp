#include "WPDLLModifier.h"

WPDLLModifier::WPDLLModifier() :
    WPModifier(),
    modifier(0),
    manager(0)
{
}

WPDLLModifier::~WPDLLModifier()
{
    if (manager)
    {
        if (modifier)
            manager->deleteObject(modifier);
        delete manager;
    }
    //manager = 0;
}

bool WPDLLModifier::loadDLL(std::string name)
{
    if (manager)
        delete manager;
    //manager = 0;
    manager = new WPDLLManager();
    if (!manager->openDLL(name))
        return false;
    if (!manager->sendCallbackHandle())
    {
        qWarning("No setCallback !");
        return false;
    }
    modifier = (WPModifier *)manager->newObject();
    if (!modifier)
        return false;
    return true;
}

void WPDLLModifier::reset()
{
    modifier->reset();
}

void WPDLLModifier::setNotes(const std::vector<WPNote> &_notes)
{
    modifier->setNotes(_notes);
}

void WPDLLModifier::setNote(const WPNote &_note)
{
    modifier->setNote(_note);
}

void WPDLLModifier::set(std::string para)
{
    modifier->set(para);
}

bool WPDLLModifier::isGlobal()
{
    return modifier->isGlobal();
}

bool WPDLLModifier::isSingleNote()
{
    return modifier->isSingleNote();
}

bool WPDLLModifier::isTuning()
{
    return modifier->isTuning();
}

bool WPDLLModifier::isTimbreModifier()
{
    return modifier->isTimbreModifier();
}

bool WPDLLModifier::isTempoModifier()
{
    return modifier->isTempoModifier();
}

bool WPDLLModifier::isNoteModifier()
{
    return modifier->isNoteModifier();
}

bool WPDLLModifier::isFreqModifier()
{
    return modifier->isFreqModifier();
}

bool WPDLLModifier::isAmpModifier()
{
    return modifier->isAmpModifier();
}

std::string WPDLLModifier::modifyTimbre()
{
    return modifier->modifyTimbre();
}

double WPDLLModifier::modifyTempo(double time, double tempo)
{
    return modifier->modifyTempo(time, tempo);
}

double WPDLLModifier::modifyNote(double time)
{
    return modifier->modifyNote(time);
}

double WPDLLModifier::modifyFreq(double time, double freq)
{
    return modifier->modifyFreq(time, freq);
}

double WPDLLModifier::modifyAmp(double time, double amp)
{
    return modifier->modifyAmp(time, amp);
}
