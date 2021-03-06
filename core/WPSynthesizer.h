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
#include "WPScore/WPNote.h"
#include "WPScore/WPPart.h"
class WPSynthesizer;
#include "WPDLLManager/WPDLLModifier.h"
#include "WPDLLManager/WPDLLTimbreManager.h"
#include "plugins/WP12EqualTuning.h"
#include "plugins/WPDefaultNoteModifier.h"

class WPSynthesizer : public QThread
{
    Q_OBJECT
public:
    static const double TimeStep = 1.0 / 1200.0;  //used to be 1200
    static WPWave::WaveDataType truncateWaveData(double x);

    explicit WPSynthesizer(QObject *parent = 0);
    ~WPSynthesizer();

    void setOutputDevice(QIODevice &_output);
    void startSynthesis(WPPart &_part);
    void setPart(WPPart &_part);
    void setWaitingTime(int msec);

signals:
    void synthesisFinished();
    
public slots:

protected:
    void run();

private slots:
    void synthesizePartLoopBody();
    void outputNoteLoopBody();

private:
    void synthesizePartInitialize();
    void synthesizePartFinalize();
    void synthesizePartLoopInitialize();
    void synthesizePartLoopFinalize();
    void scheduleSynthesizePartLoopBody();

    void outputNoteInitialize();
    void outputNoteFinalize();
    void scheduleOutputNoteLoopBody();

    void processProperties(double time0, double time1, std::vector<WPProperty> &sprop, std::vector<WPProperty> &eprop);
    void insertProperty(WPProperty prop);
    void removeProperty(WPProperty prop);
    int processTuningFreqAmp(double time, std::vector<double> &freq, std::vector<double> &amp);
    int processNote(double time, double &notelength);
    int processTimbre(double time, std::string &timbrename);
    int processTempo(double time, double &tempo);
    void processAllModifiers(double time, std::vector<double> &freq, std::vector<double> &amp, double &notelength, double &tempo, std::string &timbre);
    //freq should be filled with 0's
    void outputNote();
    std::map<WPProperty, WPPropertyAndModifiers *> propmap;
    QIODevice *output;
    WPPart *part;
    WP12EqualTuning defaulttuning;
    WPDefaultNoteModifier defaultnotemodifier;

    std::pair<WPMultinote, std::pair<std::vector<WPProperty>, std::vector<WPProperty> > > fragment;
    WPWave *swave;
    WPWave *twave;
    std::string timbrename;
    double time0, time1;
    std::vector<std::vector<double> > freq, amp;
    std::vector<double> tempo, time;
    std::vector<double> vtime;
    int samplei, samplecnt;
    double notelength;
    double timeend;
    std::vector<WPTimbre *> timbre;
    WPDLLTimbreManager timbremanager;
    double dur;

    std::vector<WPNote> notes;
    std::vector<WPProperty> sprop, eprop;
    WPPropertyAndModifiers *pam;

    QTimer timer, outputtimer;
    int waitingtime;

    int total = 0;

};

#endif // WPSYNTHESIZER_H
