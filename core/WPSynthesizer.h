#ifndef WPSYNTHESIZER_H
#define WPSYNTHESIZER_H

#include <sstream>
#include <set>
#include <QObject>
#include <QTimer>
#include <QThread>
#include "WPWave.h"
#include "WPTimbre.h"
#include "WPModifier.h"
#include "WPPropertyAndModifiers.h"
#include "../WPScore/WPNote.h"
#include "../WPScore/WPPart.h"
#include "WPDLLManager/WPDLLModifier.h"

class WPSynthesizer : public QThread
{
    Q_OBJECT
public:
    //static WPWave *waveTuningFork(double frequency, double duration);
    static const double TimeStep = 1.0 / 1200.0;
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
    int processTuningFreqAmp(int nnote, double time, double &freq, double &amp);
    void processFreqAmpMultiple(double time, std::vector<double> &freq, std::vector<double> &amp);
    int processNote(double time, double &notelength);
    std::map<WPProperty, WPPropertyAndModifiers> propmap;
    const WPTimbre *timbre;
    QIODevice *output;
    WPPart *part;
    bool slowingdown;

};

#endif // WPSYNTHESIZER_H
