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
    std::set<WPPropertyAndModifiers> cpropset;
    std::map<WPProperty, WPPropertyAndModifiers> propmap;
    WPWave *swave = new WPWave;
    //fflush(stdout);
    //fflush(stderr);
    cmodi.clear();
    while (fragment = part->nextFragment(), !(fragment.first.getLength() == Fraction(-1, 1)))
    {
        std::vector<WPNote> notes = fragment.first.getNotes();
        std::vector<WPNote>::iterator noteiter;
        std::vector<WPProperty> sprop = fragment.second.first, eprop = fragment.second.second;
        std::vector<WPProperty>::iterator propiter;
        std::map<WPProperty, WPPropertyAndModifiers>::iterator propmapiter;

        double freq0, freq1;
        double amp0, amp1;
        double tempo0, tempo1;
        //scan time
        double time0, time1, timespan;
        timespan = notes[0].getLength().toDouble();
        for (time0 = 0.0, time1 = 1 / TimeStep;
             time0 < timespan;
             time0 = time1, time1 = std::min(time1 + 1.0 / WPTimbre::ControlRate, timespan))
        {
            //process properties
            //delete ended
            for (propiter = eprop.begin(); propiter != eprop.end(); propiter++)
                if (/*already begun*/)
                {
                    //cpropsetiter = cpropset.find(WPPropertyAndModifiers(*propiter));
                    //cpropset.erase(cporpsetiter);
                    propmap.erase(*propiter);
                }
            //insert starting
            for (propiter = sprop.begin(); propiter != sprop.end(); propiter++)
                if (/*already ended*/)
                {
                    WPPropertyAndModifiers *pam = new WPPropertyAndModifiers;
                    if (pam->setProperty(*propiter))
                    {
                        propmap.insert(*propiter, *pam);
                    }
                    else
                    {
                        qWarning() << "No plug-in \"" << pam.getName() << "\"!";
                    }
                    delete pam;
                }
            //Tuning Properties
            //Amp
            //Freq
            //Note
            //Tempo
            swave->clear();
            //Timbre
            if (output->write((char *)swave->data.begin(), swave->data.size() * sizeof(WPWave::WaveDataType)) == -1)
            {
                qCritical() << errorString();
            }
            QThread::usleep(10000); //remember to remove
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
