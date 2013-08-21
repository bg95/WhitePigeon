#ifndef WPSYNTHESIZER_H
#define WPSYNTHESIZER_H

#include <QObject>
#include <QTimer>
#include <QThread>
#include "WPWave.h"
#include "WPTimbre.h"
#include "WPModifier.h"
#include "../WPScore/WPNote.h"
#include "../WPScore/WPPart.h"
#include "WPDLLManager/WPDLLModifier.h"

class WPSynthesizer : public QThread
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
    void setPart(WPPart &_part);

signals:
    void synthesisFinished();
    
public slots:
    void slowDown();

protected:
    void run();

private slots:
    void synthesizePart();

private:
    const WPTimbre *timbre;
    QIODevice *output;
    WPPart *part;
    bool slowingdown;

};

#endif // WPSYNTHESIZER_H
