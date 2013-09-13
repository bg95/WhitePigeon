#include "WPSynthesisController.h"

WPSynthesisController::WPSynthesisController(QObject *parent) :
    QObject(parent),
    synthesizer(0),
    mixer(0),
    outpipe(0),
    audiooutput(0),
    //tuningfork(new WPTuningFork),
    lock(QMutex::NonRecursive)//,
    //dlltimbre(new WPDLLTimbre)
{/*
    if (!dlltimbre->loadDLL("/home/pt-cr/Projects/build-WhitePigeon-Desktop-Debug/plugins/WPTimbre/libWPTuningFork.so"))
        qWarning("unable to open dlltimbre");*/
}

WPSynthesisController::~WPSynthesisController()
{
    if (synthesizer)
        delete[] synthesizer;
    qDebug("WPSynthesisController: Synthesizers deleted");
    delete mixer;
    qDebug("WPSynthesisController: Mixer deleted");
    delete outpipe;
    delete audiooutput;
    //delete tuningfork;
    //delete dlltimbre;
}

void WPSynthesisController::synthesizeAndOutput(WPScore &score, QIODevice *output)
{
    qDebug("SynthesisController %X in thread %X\n", (quint64)this, (quint64)QThread::currentThread());
    int i;
    partnum = score.getPartList().size();

    if (synthesizer)
        delete[] synthesizer;
    if (mixer)
        delete mixer;
    synthesizer = new WPSynthesizer[partnum];
    mixer = new WPMixer;
/*
    synthesizer[0].setOutputDevice(*output);
    synthesizer[0].setPart(score.getPartList()[0]);
    connect(&synthesizer[0], SIGNAL(synthesisFinished()), this, SLOT(oneSynthesizerFinished()));
    connect(&synthesizer[0], SIGNAL(synthesisFinished()), this, SLOT(mixerFinished()));
    synthesizer[0].start();
*/
    mixer->setWaitingTime(100);
    mixer->setOutput(*output);
    mixer->setReadLength(8000);
    mixer->openInputChannels(partnum);
    for (i = 0; i < partnum; i++)
    {
        synthesizer[i].setOutputDevice(*mixer->getInputChannel(i));
        //synthesizer[i].loadTimbre(dlltimbre);
        //synthesizer[i].loadTimbre(tuningfork); //load tuningfork for test
        synthesizer[i].setPart(score.getPartList()[i]);
        synthesizer[i].setWaitingTime(100);
        connect(&synthesizer[i], SIGNAL(synthesisFinished()), this, SLOT(oneSynthesizerFinished()));
        //connect(&synthesizer[i], SIGNAL(finished()), &synthesizer[i], SLOT(deleteLater()));
        //connect(mixer->getInputChannel(i), SIGNAL(sufficientInput()), &synthesizer[i], SLOT(slowDown()));
        mixer->getInputChannel(i)->setThresholds(0, 160000);
    }
    connect(mixer, SIGNAL(allInputClosed()), this, SLOT(mixerFinished()));
    for (i = 0; i < partnum; i++)
        //synthesizer[i].startSynthesis(score.getPartList()[i]);
        synthesizer[i].start();
    mixer->start();
}

QIODevice *WPSynthesisController::synthesize(WPScore &score) //don't run two threads in this function simultaneously
{
    if (outpipe)
        delete outpipe;
    outpipe = new WPPipe;
    outpipe->open(QIODevice::ReadWrite);
    synthesizeAndOutput(score, outpipe);
    return outpipe;
}

void WPSynthesisController::synthesizeAndPlay(WPScore &score)
{
    if (audiooutput)
        delete audiooutput;
    audiooutput = new QAudioOutput(WPWave::defaultAudioFormat());
    //connect(audiooutput, SIGNAL(stateChanged(QAudio::State)), this, SLOT(audiooutputStateChanged(QAudio::State)));
    audiooutput->start(synthesize(score));
}

void WPSynthesisController::stopPlaying()
{
    if (!audiooutput)
        return;
    if (audiooutput->state() != QAudio::StoppedState)
        audiooutput->stop();
    emit playingStopped();
}

void WPSynthesisController::stopAll()
{
    recycle();
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
    if (mixer->getInputChannel(i)->isOpen())
        qDebug("closing input %d, %d", i, mixer->getInputChannel(i)->open(QIODevice::ReadOnly));

}

void WPSynthesisController::mixerFinished()
{
    qDebug("mixer finished");
    delete[] synthesizer;
    synthesizer = 0;
    //for (int i = 0; i < partnum; i++)
    //    synthesizer[i].deleteLater();
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
    delete mixer;
    mixer = 0;
    delete outpipe;
    outpipe = 0;
    delete audiooutput;
    audiooutput = 0;
}
