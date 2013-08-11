#ifndef WPSYNTHESISCONTROLLER_H
#define WPSYNTHESISCONTROLLER_H

#include <QObject>
#include <QIODevice>
#include <QAudioOutput>
#include <QMutex>
#include "WPSynthesizer.h"
#include "WPPipe.h"
#include "WPMixer.h"
#include "WPTuningFork.h"
#include "../WPScore/WPScore.h"

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
    
signals:
    void synthesisFinished();
    
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
    QFile *file;
    QMutex lock;
    int partnum;

    WPTuningFork *tuningfork; //for test
    
};

#endif // WPSYNTHESISCONTROLLER_H
