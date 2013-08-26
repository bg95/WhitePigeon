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
    WPWave *swave = new WPWave;

    double time0, time1;
    std::vector<std::vector<double> > freq, amp;
    std::vector<double> tempo, time, vtime;
    double dur;
    int samplecnt;

    samplecnt = 0;
    freq.clear();
    amp.clear();
    tempo.clear();
    time.clear();
    time0 = 0.0;
    time1 = TimeStep;
    propmap.clear();
    samplecnt = 0;

    freq.push_back(std::vector<double>());
    amp.push_back(std::vector<double>());
    tempo.push_back(0.0);
    time.push_back(time0);
    processAll(time0, freq[samplecnt], amp[samplecnt], notelength, tempo[samplecnt]);
    samplecnt++;

    while (fragment = part->nextFragment(), !(fragment.first.getLength() == Fraction(-1, 1)))
    {
        std::vector<WPNote> notes = fragment.first.getNotes();
        std::vector<WPProperty> sprop = fragment.second.first, eprop = fragment.second.second;

        double notelength;
        //scan time
        double timeend;
        timeend = notes[0].getLength().toDouble() + time0;
        for (;
             time0 < timeend;
             time0 = time1, time1 = std::min(time1 + TimeStep, timeend))
        {
            processProperties(time0, time1, sprop, eprop);

            freq.push_back(std::vector<double>());
            amp.push_back(std::vector<double>());
            tempo.push_back(0.0);
            time.push_back(time1);
            processAll(time1, freq[samplecnt], amp[samplecnt], notelength, tempo[samplecnt]);
            samplecnt++;

            if (notelength >= 0)
            {
                //Timbre
                //calculate vtime and dur
                //for ...
                {
                    swave->clear();
                    if (output->write((char *)swave->data.begin(), swave->data.size() * sizeof(WPWave::WaveDataType)) == -1)
                    {
                        qCritical() << errorString();
                    }
                    QThread::usleep(100); //remember to remove
                }
            }
        }
        time1 = time0 + TimeStep;
    }
    //swave->setFormat(WPWave::defaultAudioFormat());
    //swave->play();
    delete swave;
    //output->open(output->openMode() & (~QIODevice::WriteOnly));
    synthesisFinished();
}

//private

void WPSynthesizer::processProperties(double time0, double time1, const std::vector<WPProperty> &sprop, const std::vector<WPProperty> &eprop)
{
    std::vector<WPProperty>::iterator propiter;
    //process properties
    //delete ended
    for (propiter = eprop.begin(); propiter != eprop.end(); propiter++)
        if (/*already ended*/)
        {
            //cpropsetiter = cpropset.find(WPPropertyAndModifiers(*propiter));
            //cpropset.erase(cporpsetiter);
            propmap.erase(*propiter);
        }
    //insert starting
    for (propiter = sprop.begin(); propiter != sprop.end(); propiter++)
        if (/*already begun*/)
        {
            WPPropertyAndModifiers *pam = new WPPropertyAndModifiers;
            if (pam->setProperty(*propiter))
            {
                propmap.insert(*propiter, *pam);
                /*
                if (pam->sampleModifier()->isSingleNote() &&
                        (pam->sampleModifier()->isFreqModifier() ||
                         pam->sampleModifier()->isAmpModifier()))
                    pam->setNumModifiers(notes.size());
                */
                pam->sampleModifier()->setNotes(part->getNotesByInterval((*propiter).getInterval()));
                pam->sampleModifier()->reset((*propiter).getInterval().begin().getValue().toDouble());
                //pam->sampleModifier()->set();
            }
            else
            {
                qWarning() << "No plug-in \"" << pam.getName() << "\"!";
            }
            delete pam;
        }
}


int WPSynthesizer::processTuningFreqAmp(double time, std::vector<double> &freq, std::vector<double> &amp)
{
    std::map<WPProperty, WPPropertyAndModifiers>::iterator propmapiter;
    int tuned = 0;
    for (propmapiter = propmap.begin(); propmapiter != propmap.end(); propmap++)
        if ((*propmapiter).second.sampleModifier()->isTuning())
        {
            freq = (*propmapiter).second.sampleModifier()->modifyFreq(time);
            tuned++;
        }
    for (propmapiter = propmap.begin(); propmapiter != propmap.end(); propmap++)
        if ((*propmapiter).second.sampleModifier()->isFreqModifier() &&
            !(*propmapiter).second.sampleModifier()->isTuning())
        {
            freq = (*propmapiter).second.sampleModifier()->modifyFreq(time, freq);
        }
    for (propmapiter = propmap.begin(); propmapiter != propmap.end(); propmap++)
        if ((*propmapiter).second.sampleModifier()->isAmpModifier())
        {
            amp = (*propmapiter).second.sampleModifier()->modifyAmp(time, amp);
        }
    return tuned;
}

int WPSynthesizer::processNote(double time, double &notelength)
{
    double tmp;
    std::map<WPProperty, WPPropertyAndModifiers>::iterator propmapiter;
    int notemodicnt = 0;
    notelength = -1.0;
    for (propmapiter = propmap.begin(); propmapiter != propmap.end(); propmap++)
        if ((*propmapiter).second.sampleModifier()->isNoteModifier())
        {
            tmp = (*propmapiter).second.sampleModifier()->modifyNote(time);
            if (tmp > 0)
                notelength = tmp;
            notemodicnt++;
        }
    return notemodicnt;
}

void WPSynthesizer::processTempo(double time, double &tempo)
{
    std::map<WPProperty, WPPropertyAndModifiers>::iterator propmapiter;
    for (propmapiter = propmap.begin(); propmapiter != propmap.end(); propmap++)
        if ((*propmapiter).second.sampleModifier()->isTempoModifier())
        {
            tempo = (*propmapiter).second.sampleModifier()->modifyTempo(time, tempo);
        }
}

void WPSynthesizer::processAllModifiers(double time, std::vector<double> &freq, std::vector<double> &amp, double &notelength, double &tempo)
{
    std::map<WPProperty, WPPropertyAndModifiers>::iterator propmapiter;
    for (propmapiter = propmap.begin(); propmapiter != propmap.end(); propmap++)
        (*propiter).second.sampleModifier()->setTime(time);
    //Tuning
    //Freq
    //Amp
    tuned = processTuningFreqAmp(time, freq, amp);
    if (tuned == 0)
        qWarning("No Tuning!");
    if (tuned >= 2)
        qWarning("Multiple Tuning!");
    //Note
    notemodicnt = processNote(time, notelength);
    if (notemodicnt == 0)
        qWarning("No note modifier!");
    if (notemodicnt >= 2)
        qWarning("Multiple note modifier!");
    //Tempo
    processTempo(time, tempo);
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
