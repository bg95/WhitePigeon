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
    WPWave *swave = new WPWave;
    //fflush(stdout);
    //fflush(stderr);
    cmodi.clear();
    propmap.clear();
    while (fragment = part->nextFragment(), !(fragment.first.getLength() == Fraction(-1, 1)))
    {
        std::vector<WPNote> notes = fragment.first.getNotes();
        std::vector<WPNote>::iterator noteiter;
        std::vector<WPProperty> sprop = fragment.second.first, eprop = fragment.second.second;
        std::vector<WPProperty>::iterator propiter;
        std::map<WPProperty, WPPropertyAndModifiers>::iterator propmapiter;

        std::vector<std::vector<double> > freq, amp;
        std::vector<double> tempo, time;
        std::vector<double> freq0, freq1;
        std::vector<double> amp0, amp1;
        double tempo0, tempo1;
        int tuned, notemodicnt;
        double notelength;
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
                        if (pam->sampleModifier()->isSingleNote() &&
                                (pam->sampleModifier()->isFreqModifier() ||
                                 pam->sampleModifier()->isAmpModifier()))
                            pam->setNumModifiers(notes.size());
                    }
                    else
                    {
                        qWarning() << "No plug-in \"" << pam.getName() << "\"!";
                    }
                    delete pam;
                }
            //Tuning Properties
            //Freq
            //Amp
            tuned = 0;
            for (i = 0; i < notes.size(); i++)
                tuned = processTuningFreqAmp(i, time1, freq1[i], amp1[i]);
            if (tuned == 0)
                qWarning("No Tuning!");
            if (tuned >= 2)
                qWarning("Multiple Tuning!");
            processFreqAmpMultiple(time, freq1, amp1);
            //Note
            notemodicnt = processNote(time, notelength);
            if (notemodicnt == 0)
                qWarning("No note modifier!");
            if (notemodicnt >= 2)
                qWarning("Multiple note modifier!");
            //Tempo
            processTempo(time, tempo1);
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

//private

int WPSynthesizer::processTuningFreqAmp(int nnote, double time, double &freq, double &amp)
{
    std::map<WPProperty, WPPropertyAndModifiers>::iterator propmapiter;
    int tuned = 0;
    for (propmapiter = propmap.begin(); propmapiter != propmap.end(); propmap++)
        if ((*propmapiter).second.sampleModifier()->isTuning() &&
            (*propmapiter).second.sampleModifier()->isSingleNote())
        {
            freq = (*propmapiter).second.modifiers[nnote]->modifyFreq(time);
            tuned++;
        }
    for (propmapiter = propmap.begin(); propmapiter != propmap.end(); propmap++)
        if ((*propmapiter).second.sampleModifier()->isFreqModifier() &&
            (*propmapiter).second.sampleModifier()->isSingleNote() &&
            !(*propmapiter).second.sampleModifier()->isTuning())
        {
            freq = (*propmapiter).second.modifiers[nnote]->modifyFreq(time, freq);
        }
    for (propmapiter = propmap.begin(); propmapiter != propmap.end(); propmap++)
        if ((*propmapiter).second.sampleModifier()->isAmpModifier() &&
            (*propmapiter).second.sampleModifier()->isSingleNote())
        {
            amp = (*propmapiter).second.modifiers[nnote]->modifyAmp(time, amp);
        }
    return tuned;
}

void WPSynthesizer::processFreqAmpMultiple(double time, std::vector<double> &freq, std::vector<double> &amp)
{
    std::map<WPProperty, WPPropertyAndModifiers>::iterator propmapiter;
    for (propmapiter = propmap.begin(); propmapiter != propmap.end(); propmap++)
        if ((*propmapiter).second.sampleModifier()->isFreqModifier() &&
            !(*propmapiter).second.sampleModifier()->isSingleNote())
        {
            freq = (*propmapiter).second.sampleModifier()->modifyFreq(time, freq);
        }
    for (propmapiter = propmap.begin(); propmapiter != propmap.end(); propmap++)
        if ((*propmapiter).second.sampleModifier()->isAmpModifier() &&
            !(*propmapiter).second.sampleModifier()->isSingleNote())
        {
            amp = (*propmapiter).second.sampleModifier()->modifyAmp(time, amp);
        }
}

int WPSynthesizer::processNote(double time, double &notelength)
{
    double tmp;
    std::map<WPProperty, WPPropertyAndModifiers>::iterator propmapiter;
    int notemodicnt = 0;
    notelength = -1.0;
    for (propmapiter = propmap.begin(); propmapiter != propmap.end(); propmap++)
        if ((*propmapiter).second.sampleModifier()->isNoteModifier() &&
            (*propmapiter).second.sampleModifier()->isSingleNote())
        {
            tmp = (*propmapiter).second.sampleModifier()->modifyNote(time);
            if (tmp > 0)
                notelength = tmp;
            notemodicnt++;
        }
    for (propmapiter = propmap.begin(); propmapiter != propmap.end(); propmap++)
        if ((*propmapiter).second.sampleModifier()->isNoteModifier() &&
            !(*propmapiter).second.sampleModifier()->isSingleNote())
        {
            tmp = (*propmapiter).second.sampleModifier()->modifyNote(time);
            if (tmp > 0)
                notelength = tmp;
            notemodicnt++;
        }
    return notemodicnt;
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
