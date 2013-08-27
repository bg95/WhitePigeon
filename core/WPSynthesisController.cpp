#include "WPSynthesisController.h"

WPSynthesisController::WPSynthesisController(QObject *parent) :
    QObject(parent),
    synthesizer(0),
    mixer(new WPMixer),
    outpipe(new WPPipe),
    audiooutput(new QAudioOutput(WPWave::defaultAudioFormat())),
    tuningfork(new WPTuningFork),
    lock(QMutex::NonRecursive)
{
}

WPSynthesisController::~WPSynthesisController()
{
    delete mixer;
    delete outpipe;
    delete audiooutput;
    delete tuningfork;
}

void WPSynthesisController::synthesizeAndOutput(WPScore &score, QIODevice *output)
{
    int i;
    partnum = score.getPartList().size();

    recycle();
    synthesizer = new WPSynthesizer[partnum];

    mixer->setWatingTime(10);
    mixer->setOutput(*output);
    mixer->setReadLength(8000);
    mixer->openInputChannels(partnum);
    for (i = 0; i < partnum; i++)
    {
        synthesizer[i].setOutputDevice(*mixer->getInputChannel(i));
        synthesizer[i].loadTimbre(tuningfork); //load tuningfork for test
        connect(&synthesizer[i], SIGNAL(synthesisFinished()), this, SLOT(oneSynthesizerFinished()));
    }
    connect(mixer, SIGNAL(allInputClosed()), this, SLOT(mixerFinished()));
    for (i = 0; i < partnum; i++)
        synthesizer[i].startSynthesis(score.getPartList()[i]);
    mixer->start();
/*    synthesizer[0].setOutputDevice(*output);
    synthesizer[0].loadTimbre(tuningfork);
    connect(&synthesizer[0], SIGNAL(synthesisFinished()), this, SLOT(mixerFinished()));
    synthesizer[0].startSynthesis(score.getPartList()[0]);*/
}

QIODevice *WPSynthesisController::synthesize(WPScore &score) //don't run two threads in this function simultaneously
{
    outpipe->open(QIODevice::ReadWrite);
    synthesizeAndOutput(score, outpipe);
    return outpipe;
}

void WPSynthesisController::synthesizeAndPlay(WPScore &score)
{
    //connect(audiooutput, SIGNAL(stateChanged(QAudio::State)), this, SLOT(audiooutputStateChanged(QAudio::State)));
    audiooutput->start(synthesize(score));
}

void WPSynthesisController::stopPlaying()
{
    if (audiooutput->state() != QAudio::StoppedState)
        audiooutput->stop();
}

void WPSynthesisController::oneSynthesizerFinished()
{
    if (sender() == 0)
    {
        qCritical("non-signal call of oneSynthesizerFinished");
        return;
    }
    quint32 i = (WPSynthesizer *)sender() - synthesizer;
    qDebug("synthesizer %d finished", i);
    qDebug("closing input %d, %d", i, mixer->getInputChannel(i)->open(QIODevice::ReadOnly));
}

void WPSynthesisController::mixerFinished()
{
    qDebug("mixer finished");
    delete[] synthesizer;
    if (outpipe->isOpen())
        outpipe->closeInput();
    emit synthesisFinished();
}

void WPSynthesisController::audiooutputStateChanged(QAudio::State state)
{
    switch (state)
    {
    case QAudio::IdleState:
        stopPlaying();
        break;
    case QAudio::StoppedState:
        if (audiooutput->error() != QAudio::NoError)
        {
            qDebug("error! %d", audiooutput->error());
        }
        break;
    }
}

void WPSynthesisController::recycle()
{
    if (synthesizer)
    {
        delete[] synthesizer;
        synthesizer = 0;
    }
}
