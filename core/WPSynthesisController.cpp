#include "WPSynthesisController.h"

WPSynthesisController::WPSynthesisController(QObject *parent) :
    QObject(parent),
    synthesizer(0),
    mixer(0),
    outpipe(0),
    audiooutput(new QAudioOutput(WPWave::defaultAudioFormat()))
{
}

WPSynthesisController::~WPSynthesisController()
{
    delete audiooutput;
}

QIODevice *WPSynthesisController::synthesize(WPScore &score)
{
    std::vector<WPPart> &partlist = score.getPartList();
    int partnum, i;
    WPTuningFork tuningfork; //for test
    partnum = partlist.size();

    recycle();
    synthesizer = new WPSynthesizer[partnum];
    mixer = new WPMixer;
    outpipe = new WPPipe;
    outpipe->open(QIODevice::ReadWrite);

    mixer->setWatingTime(10);
    mixer->setOutput(*outpipe);
    mixer->setReadLength(1600);
    mixer->openInputChannels(partnum);
    for (i = 0; i < partnum; i++)
    {
        synthesizer[i].setOutputDevice(*mixer->getInputChannel(i));
        synthesizer[i].loadTimbre(&tuningfork); //load tuningfork for test
    }
    for (i = 0; i < partnum; i++)
        synthesizer[i].startSynthesis(partlist[i]);
    mixer->start();

    //debug
    /*
    file = new QFile("wave.out");
    file->open(QIODevice::WriteOnly);
    synthesizer[0].setOutputDevice(*file);

    synthesizer[0].setOutputDevice(*outpipe);
    synthesizer[0].loadTimbre(&tuningfork);
    //connect(&synthesizer[0], SIGNAL(synthesisFinished()), this, SLOT(synthesisFinished()));
    synthesizer[0].startSynthesis(partlist[0]);
    //audiooutput->start(outpipe);*/
    return outpipe;
}

void WPSynthesisController::synthesisFinished()
{/*
    file->close();
    delete file;
    file = new QFile("/home/pt-cr/Projects/build-WhitePigeon-Desktop-Debug/wave2.out");
    qDebug("open file wave2.out %d", file->open(QIODevice::ReadOnly));
    qDebug("format correct %d", audiooutput->format() == WPWave::defaultAudioFormat());
    audiooutput->start(file);
    qDebug("error %d", audiooutput->error());
    //delete file;
    */
}

void WPSynthesisController::synthesizeAndPlay(WPScore &score)
{
    QAudioOutput *audiooutput = new QAudioOutput(WPWave::defaultAudioFormat());
    audiooutput->start(synthesize(score));
}

void WPSynthesisController::recycle()
{
    if (synthesizer)
    {
        delete[] synthesizer;
        synthesizer = 0;
    }
    if (mixer)
    {
        delete mixer;
        mixer = 0;
    }
    if (outpipe)
    {
        delete outpipe;
        outpipe = 0;
    }
}
