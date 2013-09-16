#ifndef WPSYNTHESISCONTROLLER_H
#define WPSYNTHESISCONTROLLER_H

#include <QObject>
#include <QIODevice>
#include <QAudioOutput>
#include <QMutex>
#include "WPSynthesizer.h"
#include "WPPipe.h"
#include "WPMixer.h"
#include "../WPScore/WPScore.h"
#include "../WPDLLManager/WPDLLTimbre.h"

class WPSynthesisController : public QObject
{
    Q_OBJECT
public:
    explicit WPSynthesisController(QObject *parent = 0);
    ~WPSynthesisController();

    void synthesizeAndOutput(WPScore &score, QIODevice *output);
    QIODevice *synthesize(WPScore &score);
    void synthesizeAndPlay(WPScore &score);
    void stopPlaying();
    void stopAll();
    ///void pause();
    ///void unpause();
    
signals:
    void synthesisFinished();
    void playingStopped();
    
private slots:
    void oneSynthesizerFinished();
    void mixerFinished();
    void audiooutputStateChanged(QAudio::State state);

private:
    void recycle();
    WPSynthesizer *synthesizer;
    WPMixer *mixer;
    WPPipe *outpipe;
    QAudioOutput *audiooutput;
    //QFile *file;
    QMutex lock;
    int partnum;

    //WPTuningFork *tuningfork; //for test
    //WPDLLTimbre *dlltimbre; //for test
    
};

#endif // WPSYNTHESISCONTROLLER_H
