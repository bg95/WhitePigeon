#include "WPStaccatissimo.h"

WPStaccatissimo::WPStaccatissimo()
	: Shortened(0.25)
{
}

WPStaccatissimo::~WPStaccatissimo()
{
}

/*
void WPStaccatissimo::setNotes(WPMultinote *notes, int num, double offset)
{
    WPModifier::setNotes(notes, num, offset);
    int i;
    notestart.clear();
    notestart.push_back(0.0);
    for (i = 0; i < num; i++)
    	notestart.push_back(((notes + i)->*getLengthDouble)() + notestart[i]);
    notestart.pop_back();
}*/
void WPStaccatissimo::reset()
{
	length = getNotes()[0].getLength().toDouble();
}

double WPStaccatissimo::modifyNote(double time)
{
    if (timePassed(0.0 + 1E-9))
        return length * Shortened;
    if (timePassed(length * Shortened))
        return length * (1.0 - Shortened);
}

std::vector<double> WPStaccatissimo::modifyAmp(double time, std::vector<double> amp)
{
	printf("WPStaccatissimo::modifyAmp length*Shortened=%.2lf getTime()=%.2lf\n", length * Shortened, getTime());
	std::vector<double>::iterator iter;
	double rtime = getTime();
    if (rtime < length * Shortened)
    {
		//double t = std::exp(rtime / (length * Shortened));
		double t = 1.0;
		for (iter = amp.begin(); iter != amp.end(); iter++)
			(*iter) *= t;
		return amp;
    }
	for (iter = amp.begin(); iter != amp.end(); iter++)
		(*iter) = 0.0;
	return amp;
}

extern "C"
{
    WPModifier *create()
    {
        return new WPStaccatissimo();
    }
    void destroy(WPModifier *p)
    {
        fflush(stdout);
        if (p)
            delete p;
    }
}
