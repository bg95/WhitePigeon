#include "WPDefaultNoteModifier.h"

//void *(*WPDefaultNoteModifier::callback)(WPCallbackManager::CallbackFuncStatic) = 0;

WPDefaultNoteModifier::WPDefaultNoteModifier()
{
}

WPDefaultNoteModifier::~WPDefaultNoteModifier()
{
}

//void WPDefaultNoteModifier::setNotes(const std::vector<WPMultinote> &notes, double offset)
void WPDefaultNoteModifier::reset()
{
    std::vector<WPMultinote>::const_iterator iter, end;
    int i;
    //...
    double offset = getNotesOffset();
    notestarts.clear();
    notestarts.push_back(-offset/*.toDouble()*/);
    i = notestarts.size();
    end = getNotes().end();
    for (iter = getNotes().begin(); iter != end; iter++)
    {
        notestarts.push_back(notestarts[i - 1] + (*iter).getLength().toDouble());
        i++;
    }
    notestarts.pop_back();
    notei = 0;
}

double WPDefaultNoteModifier::modifyNote(double time)
{
    std::vector<WPMultinote> multinotes = getNotes();
    for (; notestarts[notei] <= getTime() && notei < notestarts.size(); notei++)
        if (timePassed(notestarts[notei]))
            return multinotes[notei].getNotes()[0].getLength().toDouble();
    return -1;
}

std::string WPDefaultNoteModifier::getComment()
{
    return std::string("The default note modifier. Does not modify the notes.");
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
