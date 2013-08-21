#include "WPSynthesizer.h"

WPSynthesizer::WPSynthesizer(QObject *parent) :
    QThread(parent)
{
}

WPSynthesizer::WPSynthesizer(WPTimbre *_timbre, QObject *parent) :
    QThread(parent)
{
    loadTimbre(_timbre);
}

WPSynthesizer::~WPSynthesizer()
{
    if (isRunning())
        qDebug("Synthesizer %X is still running", (quint64)this);
    quit(); //?
    qDebug("Synthesizer waiting");
    wait();
}

void WPSynthesizer::loadTimbre(const WPTimbre *_timbre)
{
    timbre = _timbre;
}

void WPSynthesizer::setOutputDevice(QIODevice &_output)
{
    output = &_output;
}

WPWave *WPSynthesizer::synthesize(WPNote &note)
{
    quint32 n = note.getTimeSpan() * WPTimbre::ControlRate, i;
    double *amp = new double[n];
    double *freq = new double[n];
    for (i = 0; i < n; i++)
        amp[i] = 0.2;
    for (i = 0; i < n; i++)
        freq[i] = note.getFrequency();
    return timbre->synthesize(note.getTimeSpan(), amp, freq); //take care of overflow
}

void WPSynthesizer::startSynthesis(WPPart &_part)
{
    part = &_part;
    qDebug("startSynthesis");
    //synthesizePart();
    //start();
}

void WPSynthesizer::setPart(WPPart &_part)
{
    part = &_part;
}

void WPSynthesizer::slowDown()
{
    //does not work
    if (!slowingdown)
    {
        slowingdown = true;
        qDebug("synthesizer slowing down");
        QThread::usleep(100000);
        slowingdown = false;
    }
}

void WPSynthesizer::run()
{
    qDebug("synthesizer run");
    slowingdown = false;
    synthesizePart();
    exec();
    qDebug("synthesizer returned from run");
}

void WPSynthesizer::synthesizePart()
{
    std::pair<WPMultinote, std::pair<std::vector<WPProperty>, std::vector<WPProperty> > > fragment;
    std::vector<WPModifier *> cmodi;
    std::vector<WPProperty> cprop;
    WPWave *swave = new WPWave;
    //fflush(stdout);
    //fflush(stderr);
    cmodi.clear();
    while (fragment = part->nextFragment(), !(fragment.first.getLength() == Fraction(-1, 1)))
    {
        std::vector<WPNote> notes = fragment.first.getNotes();
        std::vector<WPNote>::iterator iter;
        std::vector<WPProperty> sprop = fragment.second.first, eprop = fragment.second.second;
        std::vector<WPProperty>::iterator propiter;
        std::vector<WPModifier *>::iterator modiiter;
        WPDLLModifier *modifier;
        //process properties
        for (propiter = sprop.begin(); propiter != sprop.end(); propiter++)
        {
            modifier = new WPDLLModifier;
            if (modifier->loadDLL((*propiter).getNotesByInterval()))
            {
                cprop.push_back((*propiter));
                cmodi.push_back(modifier);
                if (!(*modiiter).isSingleNote())
                    (*modiiter).setNotes(part->getNotesBetween((*propiter).getInterva()));
            }
        }
        //scan time
        {
            swave->clear();
            for (iter = notes.begin(); iter != notes.end(); iter++)
            {
                WPWave *twave;
                twave = synthesize(*iter);
                swave->mixWith(1.0, *twave, 1.0);
                delete twave;
            }
            QThread::usleep(10000); //remember to remove
            if (output->write((char *)swave->data.begin(), swave->data.size() * sizeof(WPWave::WaveDataType)) == -1)
            {
                QChar *ch;
                for (ch = output->errorString().begin(); ch != output->errorString().end(); ch++)
                {
                    printf("%c", ch->toLatin1());
                }
                printf("\n");
                fflush(stdout);
            }
        }
    }
    //swave->setFormat(WPWave::defaultAudioFormat());
    //swave->play();
    delete swave;
    //output->open(output->openMode() & (~QIODevice::WriteOnly));
    synthesisFinished();
}

//static

WPWave::WaveDataType WPSynthesizer::truncateWaveData(double x)
{
    if (x > 1.0)
        x = 1.0;
    if (x < -1.0)
        x = -1.0;
    return WPWave::WaveDataType(x * 32767);
}
/*
WPWave *WPSynthesizer::waveTuningFork(double frequency, double duration)
{
    QVector<WPWave::WaveDataType> tmpdata;
    QAudioFormat format = WPWave::defaultAudioFormat();
    int i;

    tmpdata.clear();
    for (i = 0; i < duration * format.sampleRate(); i++)
    {
        double t = double(i) / double(format.sampleRate());
        tmpdata.push_back(truncateWaveData(std::exp(-1.0 * t) * std::sin(2 * WPWave::PI * frequency * t)));
    }

    return new WPWave(tmpdata, format);
}
*/
