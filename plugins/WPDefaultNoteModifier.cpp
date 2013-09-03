#include "WPDefaultNoteModifier.h"

void *(*WPDefaultNoteModifier::callback)(WPCallbackManager::CallbackFunc) = 0;

WPDefaultNoteModifier::WPDefaultNoteModifier()
{
}

WPDefaultNoteModifier::~WPDefaultNoteModifier()
{
}

void WPDefaultNoteModifier::setNotes(const std::vector<WPMultinote> &notes, Fraction offset)
{
    std::vector<WPMultinote>::iterator iter;
    int i;
	WPModifier::setNotes(notes, offset);
    //...
    notestarts.clear();
    notestarts.push_back(-offset.toDouble());
    i = notestarts.size();
    for (iter = getNotes().begin(); iter != getNotes().end(); iter++)
    {
        notestarts.push_back(notestarts[i - 1] + (*iter).getNotes()[0].getLength().toDouble());
        i++;
    }
    notestarts.pop_back();
}

double WPDefaultNoteModifier::modifyNote(double time)
{
    std::vector<WPMultinote> multinotes = getNotes();
    int i;
    for (i = 0; i < notestarts.size(); i++)
        if (timePassed(notestarts[i]))
            return multinotes[i].getNotes()[0].getLength().toDouble();
    return -1;
}
/*
extern "C"
{
    WPModifier *create()
	{
		return new WPDefaultNoteModifier();
	}
	void destroy(WPModifier *p)
	{
		printf("destroy called\n");
		fflush(stdout);
		if (p)
            delete p;
		p = 0;
	}
	void setCallback(void *_callback)
    {
        WPDefaultNoteModifier::callback = (void *(*)(WPCallbackManager::CallbackFunc))_callback;
    }
}
*/
