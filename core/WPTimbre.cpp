#include "include/core/WPTimbre.h"

const double WPTimbre::ControlRate = 480.0; //Hertz

double WPTimbre::interpolate(double t, double t0, double t1, double f0, double f1)
{
    return (t - t0) / (t1 - t0) * (f1 - f0) + f0;
}

WPTimbre::WPTimbre()
{
}

WPTimbre::~WPTimbre()
{
}

void WPTimbre::reset()
{
}

void WPTimbre::set(std::string para)
{
}
