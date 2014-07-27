#include "WPSlur.h"

WPSlur::WPSlur()
{
}

WPSlur::~WPSlur()
{
}

//void WPSlur::setNotes(WPMultinote *notes, int num, double offset)
void WPSlur::reset()
{
    //WPMultinote *iter;
	std::vector<WPMultinote>::iterator iter;

    //double (WPMultinote::*getLengthDouble)() = (typeof getLengthDouble)callMember(WPCallbackManager::WPMultinote_getLengthDouble);
    sumlength = 0.0;
    for (iter = getNotes().begin(); iter != getNotes().end(); iter++)
    {
        sumlength += (*iter).getLength().toDouble();
    }
}

double WPSlur::modifyNote(double time)
{
    if (timePassed(0.0))
        return sumlength;
    return -1;
}

extern "C"
{
    WPModifier *create()
	{
		return new WPSlur();
	}
	void destroy(WPModifier *p)
	{
		printf("destroy called\n");
		fflush(stdout);
		if (p)
            delete p;
	}
}

