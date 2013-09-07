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
class WPSynthesizer;
#include "WPDLLManager/WPDLLModifier.h"
#include "WPDLLManager/WPDLLTimbreManager.h"
#include "plugins/WP12EqualTuning.h"
#include "plugins/WPDefaultNoteModifier.h"

class WPSynthesizer : public QThread
{
    Q_OBJECT
public:
    //static WPWave *waveTuningFork(double frequency, double duration);
    static const double TimeStep = 1.0 / 1200.0;
    static WPWave::WaveDataType truncateWaveData(double x);

    explicit WPSynthesizer(QObject *parent = 0);
    //explicit WPSynthesizer(WPTimbre *_timbre, QObject *parent = 0);
    ~WPSynthesizer();

    //WPWave *synthesize(WPNote &note);
    //void loadTimbre(const WPTimbre *_timbre);
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
    void processProperties(double time0, double time1, std::vector<WPProperty> &sprop, std::vector<WPProperty> &eprop);
    int processTuningFreqAmp(double time, std::vector<double> &freq, std::vector<double> &amp);
    //void processFreqAmpMultiple(double time, std::vector<double> &freq, std::vector<double> &amp);
    int processNote(double time, double &notelength);
    int processTimbre(double time, std::string &timbrename);
    int processTempo(double time, double &tempo);
    void processAllModifiers(double time, std::vector<double> &freq, std::vector<double> &amp, double &notelength, double &tempo, std::string &timbre);
    //freq should be filled with 0's
    void outputNote();
    std::map<WPProperty, WPPropertyAndModifiers> propmap;
    QIODevice *output;
    WPPart *part;
    bool slowingdown;
    WP12EqualTuning defaulttuning;
    WPDefaultNoteModifier defaultnotemodifier;

    WPWave *swave;
    WPWave *twave;
    std::string timbrename;
    double time0, time1;
    std::vector<std::vector<double> > freq, amp;
    std::vector<double> tempo, time;
    int samplecnt;

    std::vector<WPNote> notes;
    std::vector<WPProperty> sprop, eprop;

};

#endif // WPSYNTHESIZER_H
