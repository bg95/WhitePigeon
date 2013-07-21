#ifndef WPSYNTHESIZER_H
#define WPSYNTHESIZER_H

#include <QObject>
#include "wpwave.h"
#include "../WPScore/WPNote.h"
#include "../WPScore/WPPart.h"

class WPSynthesizer : public QObject
{
    Q_OBJECT
public:
    enum TimbreType
    {
        Internal, FromFile
    };

    static WPWave *waveTuningFork(double frequency, double duration);

    explicit WPSynthesizer(QObject *parent = 0);
    explicit WPSynthesizer(TimbreType type, QString timbrename, QObject *parent = 0);

    void loadTimbre(TimbreType type, QString timbrename);
    WPWave *synthesize(WPNote &note);
    WPWave *synthesize(WPPart &part);

signals:
    
public slots:
    
private:
    WPWave *(*waveFunction)(double, double);

};

#endif // WPSYNTHESIZER_H
