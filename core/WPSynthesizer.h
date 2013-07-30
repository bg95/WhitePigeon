#ifndef WPSYNTHESIZER_H
#define WPSYNTHESIZER_H

#include <QObject>
#include "WPWave.h"
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
    void synthesize(WPPart &part);
    QAudioBuffer *getBuffer();
    void setBufferSize(quint32);
    quint32 getBufferSize() const;

signals:
    void bufferReady();
    void synthesisFinished();
    
public slots:
    
private:
    static WPWave::WaveDataType truncateWaveData(double x);

    WPWave *(*waveFunction)(double, double);
    quint32 buffersize;

};

#endif // WPSYNTHESIZER_H
