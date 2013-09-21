#ifndef WPSYNTHESIZER_H
#define WPSYNTHESIZER_H

#include <QObject>
#include <QTimer>
#include "WPWave.h"
#include "WPTimbre.h"
#include "WPScore/WPNote.h"
#include "WPScore/WPPart.h"

class WPSynthesizer : public QObject
{
    Q_OBJECT
public:
    //static WPWave *waveTuningFork(double frequency, double duration);
    static WPWave::WaveDataType truncateWaveData(double x);

    explicit WPSynthesizer(QObject *parent = 0);
    explicit WPSynthesizer(WPTimbre *_timbre, QObject *parent = 0);
    ~WPSynthesizer();

    WPWave *synthesize(WPNote &note);
    void loadTimbre(const WPTimbre *_timbre);
    void setOutputDevice(QIODevice &_output);
    void startSynthesis(WPPart &_part);

signals:
    void synthesisFinished();
    
public slots:

private slots:
    void synthesizePart();

private:
    const WPTimbre *timbre;
    QIODevice *output;
    QTimer *newthread;
    WPPart *part;

};

#endif // WPSYNTHESIZER_H
