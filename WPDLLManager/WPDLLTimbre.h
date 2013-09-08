#ifndef WPDLLTIMBRE_H
#define WPDLLTIMBRE_H

#include <QString>
#include "WPDLLManager.h"
#include "core/WPTimbre.h"

class WPDLLTimbre : public WPTimbre
{
public:
    WPDLLTimbre();
    ~WPDLLTimbre();
    bool loadDLL(QString name);
    void set(std::string para);
    void reset();
    WPWave *synthesize(double dur, double time0, double time1, double amp0, double amp1, double freq0, double freq1);
    //WPWave *synthesize(double dur, double *amp, double *freq) const;

private:
    WPTimbre *timbre;
    WPDLLManager *manager;
    WPTimbre *(*create)();
    void (*destroy)(WPTimbre *);
    void (WPTimbre::*constructor)();

};

#endif // WPDLLTIMBRE_H
