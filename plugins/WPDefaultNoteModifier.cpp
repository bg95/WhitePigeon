#include "WPDefaultNoteModifier.h"

double WPDefaultNoteModifier::setNotes(const std::vector<WPMultinote> &notes, Fraction offset)
{
	WPModifier::setNotes(notes, offset);
	...
}

double WPDefaultNoteModifier::modifiNote(double time)
{
    std::vector<WPMultiNote> multinotes = getNotes();
    ...
}

extern "C"
{
	WPTimbre *create()
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
