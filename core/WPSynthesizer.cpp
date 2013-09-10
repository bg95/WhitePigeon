#include "WPSynthesizer.h"

WPSynthesizer::WPSynthesizer(QObject *parent) :
    QThread(parent)
{
}
/*
WPSynthesizer::WPSynthesizer(WPTimbre *_timbre, QObject *parent) :
    QThread(parent)
{
    loadTimbre(_timbre);
}
*/
WPSynthesizer::~WPSynthesizer()
{
    if (isRunning())
        qDebug("Synthesizer %X is still running", (unsigned)(quint64)this);
    quit(); //?
    qDebug("Synthesizer waiting");
    wait();
}
/*
void WPSynthesizer::loadTimbre(const WPTimbre *_timbre)
{
    timbre = _timbre;
}
*/
void WPSynthesizer::setOutputDevice(QIODevice &_output)
{
    output = &_output;
}
/*
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
*/
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
    qDebug("Synthesizer %X in thread %X\n", (quint64)this, (quint64)QThread::currentThread());
    slowingdown = false;
    synthesizePart();
    exec();
    qDebug("synthesizer returned from run");
}

void WPSynthesizer::synthesizePart()
{
    std::pair<WPMultinote, std::pair<std::vector<WPProperty>, std::vector<WPProperty> > > fragment;

    swave = new WPWave;

    samplecnt = 0;
    freq.clear();
    amp.clear();
    tempo.clear();
    time.clear();
    time0 = 0.0;
    time1 = TimeStep;
    propmap.clear();

    defaultnotemodifier.setNotes(part->getAllNotes(), Fraction(0, 1));
    defaultnotemodifier.reset();
    defaulttuning.setNotes(part->getAllNotes(), Fraction(0, 1));
    defaulttuning.reset();
/*
    freq.push_back(std::vector<double>(notes.size(), 0.0));
    amp.push_back(std::vector<double>(notes.size(), 1.0));
    tempo.push_back(0.0);
    time.push_back(time0);
    processAllModifiers(time0, freq[samplecnt], amp[samplecnt], notelength, tempo[samplecnt], timbrename);
    samplecnt++;
*/
    part->lockForRead();
    fragment = part->nextFragment();
    part->unlock();
    while (!(fragment.first.getLength() == Fraction(-1, 1)))
    {
        qDebug("start processing one fragment");
        notes = fragment.first.getNotes();
        sprop = fragment.second.first;
        eprop = fragment.second.second;

        double notelength;
        //scan time
        double timeend;
        timeend = notes[0].getLength().toDouble() + time0;
        for (;
             time0 < timeend;
             time0 = time1, time1 = std::min(time1 + TimeStep, timeend))
        {
            processProperties(time0, time1, sprop, eprop);

            freq.push_back(std::vector<double>(notes.size(), 0.0));
            amp.push_back(std::vector<double>(notes.size(), part->getVolume()));
            tempo.push_back(0.0);
            time.push_back(time1);
            processAllModifiers(time1, freq[samplecnt], amp[samplecnt], notelength, tempo[samplecnt], timbrename);
            samplecnt++;

            if (notelength >= 0)
            {
                outputNote();
                samplecnt = 0;
                freq.clear();
                amp.clear();
                tempo.clear();
                time.clear();
            }
        }
        time1 = time0 + TimeStep;

        part->lockForRead();
        fragment = part->nextFragment();
        part->unlock();

    }//while

    outputNote();
    samplecnt = 0;
    freq.clear();
    amp.clear();
    tempo.clear();
    time.clear();

    //swave->setFormat(WPWave::defaultAudioFormat());
    //swave->play();
    delete swave;
    //output->open(output->openMode() & (~QIODevice::WriteOnly));
    synthesisFinished();
}

//private

void WPSynthesizer::processProperties(double time0, double time1, std::vector<WPProperty> &sprop, std::vector<WPProperty> &eprop)
{
    std::vector<WPProperty>::iterator propiter;
    double t;
    //process properties
    //delete ended
    for (propiter = eprop.begin(); propiter != eprop.end(); propiter++)
        if (/*already ended*/t = (*propiter).getInterval().end().getValue().toDouble(), time0 <= t && t < time1)
        {
            propmap.erase(*propiter);
        }
    //insert starting
    for (propiter = sprop.begin(); propiter != sprop.end(); propiter++)
        if (/*already begun*/t = (*propiter).getInterval().begin().getValue().toDouble(), time0 <= t && t < time1)
        {
            WPPropertyAndModifiers *pam = new WPPropertyAndModifiers;
            if (pam->setProperty(*propiter))
            {
                WPInterval propinterval = (*propiter).getInterval();

                part->lockForRead();
                WPInterval propexinterval = part->getExtendedInterval(propinterval);
                pam->sampleModifier()->setNotes(part->getNotesByInterval(propinterval), propinterval.begin().getValue() - propexinterval.begin().getValue());
                part->unlock();

                pam->sampleModifier()->reset();
                //propmap.insert(*propiter, *pam);
                propmap[*propiter] = *pam;
                //pam->sampleModifier()->set();
            }
            else
            {
                qWarning() << "No plug-in ";
                qWarning("%s", pam->getName().data());
            }
            delete pam;
        }
}


int WPSynthesizer::processTuningFreqAmp(double time, std::vector<double> &freq, std::vector<double> &amp)
{
    std::map<WPProperty, WPPropertyAndModifiers>::iterator propmapiter;
    int tuned = 0;
    for (propmapiter = propmap.begin(); propmapiter != propmap.end(); propmapiter++)
        if ((*propmapiter).second.sampleModifier()->isTuning())
        {
            freq = (*propmapiter).second.sampleModifier()->modifyFreq(time, freq);
            tuned++;
        }
    if (tuned == 0)
    {
        tuned++;
        freq = defaulttuning.modifyFreq(time, freq);
    }
    for (propmapiter = propmap.begin(); propmapiter != propmap.end(); propmapiter++)
        if ((*propmapiter).second.sampleModifier()->isFreqModifier() &&
            !(*propmapiter).second.sampleModifier()->isTuning())
        {
            freq = (*propmapiter).second.sampleModifier()->modifyFreq(time, freq);
        }
    for (propmapiter = propmap.begin(); propmapiter != propmap.end(); propmapiter++)
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
    for (propmapiter = propmap.begin(); propmapiter != propmap.end(); propmapiter++)
        if ((*propmapiter).second.sampleModifier()->isNoteModifier())
        {
            tmp = (*propmapiter).second.sampleModifier()->modifyNote(time);
            if (tmp > 0)
                notelength = tmp;
            notemodicnt++;
        }
    if (notemodicnt == 0)
    {
        notemodicnt++;
        notelength = defaultnotemodifier.modifyNote(time);
        return notemodicnt;
    }
    return notemodicnt;
}

int WPSynthesizer::processTempo(double time, double &tempo)
{
    int tempocnt = 0;
    std::map<WPProperty, WPPropertyAndModifiers>::iterator propmapiter;
    for (propmapiter = propmap.begin(); propmapiter != propmap.end(); propmapiter++)
        if ((*propmapiter).second.sampleModifier()->isTempoModifier())
        {
            tempo = (*propmapiter).second.sampleModifier()->modifyTempo(time, tempo);
            tempocnt++;
        }
    if (tempocnt == 0)
    {
        tempocnt++;
        tempo = 100.0;
    }
    return tempocnt;
}

int WPSynthesizer::processTimbre(double time, std::string &timbrename)
{
    int timbrecnt = 0;
    std::map<WPProperty, WPPropertyAndModifiers>::iterator propmapiter;
    for (propmapiter = propmap.begin(); propmapiter != propmap.end(); propmapiter++)
        if ((*propmapiter).second.sampleModifier()->isTimbreModifier())
        {
            timbrename = (*propmapiter).second.sampleModifier()->modifyTimbre();
            timbrecnt++;
        }
    if (timbrecnt == 0)
    {
        timbrecnt++;
        timbrename = "WPTuningFork";
    }
    return timbrecnt;
}

void WPSynthesizer::processAllModifiers(double time, std::vector<double> &freq, std::vector<double> &amp, double &notelength, double &tempo, std::string &timbrename)
{
    std::map<WPProperty, WPPropertyAndModifiers>::iterator propmapiter;
    for (propmapiter = propmap.begin(); propmapiter != propmap.end(); propmapiter++)
        (*propmapiter).second.sampleModifier()->setTime(time - (*propmapiter).first.getInterval().begin().getValue().toDouble());
    //Tuning
    //Freq
    //Amp
    defaulttuning.setTime(time);
    int tuned = processTuningFreqAmp(time, freq, amp);
    if (tuned == 0)
    {
        qWarning("No Tuning!");
        //default tuning
    }
    if (tuned >= 2)
        qWarning("Multiple Tuning!");
    //Note
    defaultnotemodifier.setTime(time);
    int notemodicnt = processNote(time, notelength);
    if (notemodicnt == 0)
    {
        qWarning("No note modifier!");
        /*defaultnotemodifier not initialized
        double tmp = defaultnotemodifier->modifyNote(time);
        if (tmp > 0)
            notelength = tmp;
        */
    }
    if (notemodicnt >= 2)
        qWarning("Multiple note modifier!");
    //Tempo
    int tempocnt = processTempo(time, tempo);
    if (tempocnt == 0)
    {
        qWarning("No Tempo!");
        //default tempo
    }
    if (tempocnt >= 2)
        qWarning("Multiple Tempo!");
    //Timbre
    int timbrecnt = processTimbre(time, timbrename);
    if (timbrecnt == 0)
    {
        qWarning("No Timbre!");
        //default timbre
    }
    if (timbrecnt >= 2)
        qWarning("Multiple Timbre!");
}

void WPSynthesizer::outputNote()
{
    std::vector<double> vtime;
    double dur;
    int i, j;
    std::vector<WPTimbre *> timbre;
    WPDLLTimbreManager timbremanager;
    if (!timbremanager.setTimbre(timbrename))
    {
        qWarning() << "Failed to open Timbre ";
        qWarning("%s", timbrename.data());
    }
    //Timbre
    //calculate vtime and dur
    dur = 0.0;
    vtime.clear();
    vtime.push_back(0.0);
    vtime.resize(samplecnt);
    for (i = 1; i < samplecnt; i++)
    {
        vtime[i] = vtime[i - 1] + (time[i] - time[i - 1]) / ((tempo[i] + tempo[i - 1]) / 2.0 / 60.0);
        dur += vtime[i] - vtime[i - 1];
    }
    timbre.clear();
    for (j = 0; j < notes.size(); j++)
    {
        timbre.push_back(timbremanager.newTimbre());
        timbre[j]->reset();
    }
    for (i = 1; i < samplecnt; i++)
    {
        swave->clear();
        for (j = 0; j < notes.size(); j++)
        {
            twave = timbre[j]->synthesize(dur, vtime[i - 1], vtime[i], amp[i - 1][j], amp[i][j], freq[i - 1][j], freq[i][j]);
            swave->mixWith(1.0, *twave, 1.0);
        }
        if (output->write((char *)swave->data.begin(), swave->data.size() * sizeof(WPWave::WaveDataType)) == -1)
        {
            qCritical() << output->errorString();
        }
        //qDebug("Synthesizer written to output %d samples", swave->data.size());
        //QThread::usleep(100); //remember to remove
    }
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
