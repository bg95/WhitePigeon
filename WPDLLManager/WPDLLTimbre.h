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
    WPWave *synthesize(double dur, double *amp, double *freq) const;

private:
    WPTimbre *timbre;
    WPDLLManager *manager;
    WPTimbre *(*create)();
    void (*destroy)(WPTimbre *);
    void (WPTimbre::*constructor)();

};

#endif // WPDLLTIMBRE_H
