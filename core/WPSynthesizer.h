#ifndef WPSYNTHESIZER_H
#define WPSYNTHESIZER_H

#include <QObject>
#include "WPWave.h"
#include "WPTimbre.h"
#include "../WPScore/WPNote.h"
#include "../WPScore/WPPart.h"

class WPSynthesizer : public QObject
{
    Q_OBJECT
public:
    //static WPWave *waveTuningFork(double frequency, double duration);
    static WPWave::WaveDataType truncateWaveData(double x);

    explicit WPSynthesizer(QObject *parent = 0);
    explicit WPSynthesizer(WPTimbre *_timbre, QObject *parent = 0);

    void loadTimbre(WPTimbre *_timbre);
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
    WPTimbre *timbre;
    //WPWave *(*waveFunction)(double, double);
    quint32 buffersize;

};

#endif // WPSYNTHESIZER_H
