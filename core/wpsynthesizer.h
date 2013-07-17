#ifndef WPSYNTHESIZER_H
#define WPSYNTHESIZER_H

#include <QObject>
#include "wpwave.h"

class WPSynthesizer : public QObject
{
    Q_OBJECT
public:
    static WPWave *waveTuningFork(double frequency, double duration);

    explicit WPSynthesizer(QObject *parent = 0);

    
signals:
    
public slots:
    
};

#endif // WPSYNTHESIZER_H
