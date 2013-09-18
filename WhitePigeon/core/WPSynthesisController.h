#ifndef WPSYNTHESISCONTROLLER_H
#define WPSYNTHESISCONTROLLER_H

#include <QObject>
#include <QIODevice>
#include <QAudioOutput>
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

    QIODevice *synthesize(WPScore &score);
    void synthesizeAndPlay(WPScore &score);
    
signals:
    
public slots:
    void synthesisFinished();

private:
    void recycle();
    WPSynthesizer *synthesizer;
    WPMixer *mixer;
    WPPipe *outpipe;
    QAudioOutput *audiooutput;
    QFile *file;
    
};

#endif // WPSYNTHESISCONTROLLER_H
