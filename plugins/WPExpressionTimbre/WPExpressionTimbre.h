#ifndef WPTUNINGFORK_H
#define WPTUNINGFORK_H

#include "core/WPTimbre.h"
#include "core/WPSynthesizer.h"

#include "WPScore/WPLib.h"

#include "Expressio/Calculator.h"

#include <cstdlib>

class WPExpressionTimbre : public WPTimbre
{
public:
    WPExpressionTimbre();
    ~WPExpressionTimbre();
    void reset();
	void set(std::string para);
    WPWave *synthesize(double dur, double time0, double time1, double amp0, double amp1, double freq0, double freq1);
    
private:
	double time, phi, phi2;
	//double envelope(double rise, double sustain, double decay, double t);
	double rand11();
	Calculator calculator;
	std::string expression;

};

#endif // WPTUNINGFORK_H
