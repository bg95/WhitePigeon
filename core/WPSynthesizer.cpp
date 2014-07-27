#include "WPSynthesizer.h"
#include "WPPipe.h"

const double WPSynthesizer::TimeStep = 1.0 / 1200.0;  //used to be 1200

WPSynthesizer::WPSynthesizer(QObject *parent) :
    QThread(parent),
    swave(0),
    twave(0),
    pam(0)
{
    timer.setSingleShot(true);
    connect(&timer, SIGNAL(timeout()), this, SLOT(synthesizePartLoopBody()));
    outputtimer.setSingleShot(true);
    connect(&outputtimer, SIGNAL(timeout()), this, SLOT(outputNoteLoopBody()));

    waitingtime = 100;
}

WPSynthesizer::~WPSynthesizer()
{
    qDebug("Deleting synthesizer %X", (quint64)this);
    if (isRunning())
        qDebug("Synthesizer %X is still running", (unsigned)(quint64)this);
    quit(); //?
    qDebug("Synthesizer %X waiting to stop", (quint64)this);
    wait();
}

void WPSynthesizer::setOutputDevice(QIODevice &_output)
{
    output = &_output;
}

void WPSynthesizer::startSynthesis(WPPart &_part)
{
    part = &_part;
    qDebug("startSynthesis");
    //synthesizePart();
    start();
}

void WPSynthesizer::setPart(WPPart &_part)
{
    part = &_part;
}

void WPSynthesizer::setWaitingTime(int msec)
{
    waitingtime = msec;
}

void WPSynthesizer::run()
{
    qDebug("synthesizer run");
    qDebug("Synthesizer %X in thread %X\n", (quint64)this, (quint64)QThread::currentThread());
    synthesizePartInitialize();
    exec();
    qDebug("synthesizer returned from run");
}

//private

void WPSynthesizer::synthesizePartInitialize()
{
    swave = new WPWave;

    samplecnt = 0;
    freq.clear();
    amp.clear();
    tempo.clear();
    time.clear();
    time0 = 0.0;
    time1 = TimeStep;
    propmap.clear();

    WPMultinote *notes;
    std::vector<WPMultinote> notesvector = part->getAllNotes();
    /*
    int num = notesvector.size();
    notes = new WPMultinote[num]; //leak!
    for (int i = 0; i < num; i++)
        notes[i] = notesvector[i];
        */
    defaultnotemodifier.setNotes(notesvector, /*Fraction(0, 1)*/0.0);
    //defaultnotemodifier.reset();
    /*
    notes = new WPMultinote[num]; //leak!
    for (int i = 0; i < num; i++)
        notes[i] = notesvector[i];
        */
    defaulttuning.setNotes(notesvector, /*Fraction(0, 1)*/0.0);
    //defaulttuning.reset();
    //memory released in synthesizerPartFinalize()
    //no need to release memory now

    part->lockForRead();
    fragment = part->nextFragment();
    part->unlock();

    synthesizePartLoopInitialize();
}

void WPSynthesizer::synthesizePartLoopInitialize()
{
    if (!(fragment.first.getLength() == Fraction(-1, 1)))
    {
        qDebug("start processing one fragment");
        notes = fragment.first.getNotes();
        sprop = fragment.second.first;
        eprop = fragment.second.second;

        timeend = notes[0].getLength().toDouble() + time0;

        notestart = true;
        scheduleSynthesizePartLoopBody();
    }
    else
        synthesizePartFinalize();
}

void WPSynthesizer::scheduleSynthesizePartLoopBody()
{
    timer.singleShot(0, this, SLOT(synthesizePartLoopBody()));
}

void WPSynthesizer::synthesizePartLoopBody()
{
    //qDebug("Synthesizer %X running in thread %X", (quint64)this, (quint64)QThread::currentThread());
    if (time0 >= timeend)
    {
        synthesizePartLoopFinalize();
        return;
    }
    processProperties(time0, time1, sprop, eprop);

    freq.push_back(std::vector<double>(notes.size(), 0.0));
    amp.push_back(std::vector<double>(notes.size(), part->getVolume()));
    tempo.push_back(0.0);
    time.push_back(time1);
    processAllModifiers(time1, freq[samplecnt], amp[samplecnt], notelength, tempo[samplecnt], timbrename);
    samplecnt++;

    if (notelength >= 0)
    {
        outputNoteInitialize();
        return;
    }

    time0 = time1;
    time1 = std::min(time1 + TimeStep, timeend);
    scheduleSynthesizePartLoopBody();
}

void WPSynthesizer::synthesizePartLoopFinalize()
{
    time1 = time0 + TimeStep;

    part->lockForRead();
    fragment = part->nextFragment();
    part->unlock();

    synthesizePartLoopInitialize();
}

void WPSynthesizer::synthesizePartFinalize()
{
    outputNote();
    samplecnt = 0;
    freq.clear();
    amp.clear();
    tempo.clear();
    time.clear();

    delete swave;
    swave = 0;

    //delete[] defaultnotemodifier.getNotes();
    //delete[] defaulttuning.getNotes();
    //resolving memory leak in synthesizePartInitialize
    //no need of delete now, for notes are copied to a vector in the modifier

    emit synthesisFinished();
}

void WPSynthesizer::processProperties(double time0, double time1, std::vector<WPProperty> &sprop, std::vector<WPProperty> &eprop)
{
    std::vector<WPProperty>::iterator propiter;
    double t;
    //process properties
    //delete ended
    for (propiter = eprop.begin(); propiter != eprop.end(); propiter++)
    {
        //qDebug("Property %s,  %lf, %lf,  current time = %lf, %lf", (*propiter).getArg().data(), (*propiter).getInterval().begin().getValue().toDouble(), (*propiter).getInterval().end().getValue().toDouble(), time0, time1);
        if (/*already ended*/t = (*propiter).getInterval().end().getValue().toDouble(), time0 <= t && t < time1)
        {
            removeProperty(*propiter);
        }
    }
    if (time1 == timeend)
    {
        for (propiter = eprop.begin(); propiter != eprop.end(); propiter++)
            if (t = (*propiter).getInterval().end().getValue().toDouble(), t == time1)
            {
                removeProperty(*propiter);
            }
    }
    //insert starting
    for (propiter = sprop.begin(); propiter != sprop.end(); propiter++)
        if (/*already begun*/t = (*propiter).getInterval().begin().getValue().toDouble(), time0 <= t && t < time1)
        {
            insertProperty(*propiter);
        }
    if (time1 == timeend)
    {
        for (propiter = sprop.begin(); propiter != sprop.end(); propiter++)
            if (t = (*propiter).getInterval().end().getValue().toDouble(), t == time1)
            {
                insertProperty(*propiter);
            }
    }
    //setNotes for WPModifier::SINGLE
    if (!notes.empty() && notestart)
    {
        notestart = false;
        std::map<WPProperty, WPPropertyAndModifiers *>::iterator propmapiter;
        for (propmapiter = propmap.begin(); propmapiter != propmap.end(); propmapiter++)
        {
            WPModifier *mod = (*propmapiter).second->sampleModifier();
            if (mod->needNotes() == WPModifier::SINGLE)
            {
                std::vector<WPMultinote> vecmlt;
                vecmlt.push_back(fragment.first); //ugly to use fragment
                mod->setNotes(vecmlt, 0.0);
            }
        }
    }
}

void WPSynthesizer::insertProperty(WPProperty prop)
{
    qDebug("insert property %s [%lf,%lf]", prop.getArg().data(), prop.getInterval().begin().getValue().toDouble(), prop.getInterval().end().getValue().toDouble());
    pam = new WPPropertyAndModifiers;
    //memory released in removeProperty()
    if (pam->setProperty(prop))
    {
        //setNotes. Another part for WPModifier::SINGLE is in synthesizePartLoopInitialize
        if (pam->sampleModifier()->needNotes() != WPModifier::NONE)
        {
            WPInterval propinterval;
            if (pam->sampleModifier()->needNotes() == WPModifier::RANGE)
                propinterval = (prop).getInterval();
            else if (pam->sampleModifier()->needNotes() == WPModifier::SINGLE)
            {
                WPPosition start = WPPosition(prop.getInterval().begin().getValue());
                propinterval = WPInterval(start, start);
            }

            part->lockForRead();
            WPInterval propexinterval = part->getExtendedInterval(propinterval);
            std::vector<WPMultinote> notesvector = part->getNotesByInterval(propexinterval);
            part->unlock();

            //int num = notesvector.size();
            //WPMultinote *notes = new WPMultinote[num]; //leak!
            //memory released in removeProperty()
            //pam->sampleModifier()->setNotes(notes, num, (propinterval.begin().getValue() - propexinterval.begin().getValue()).toDouble());
            pam->sampleModifier()->setNotes(notesvector, (propinterval.begin().getValue() - propexinterval.begin().getValue()).toDouble());
        }
        else //WPModifier::NONE, no notes passed (empty vector)
        {
            std::vector<WPMultinote> notesvector;
            pam->sampleModifier()->setNotes(notesvector, 0.0);
        }
        //pam->sampleModifier()->reset();
        //propmap.insert(prop, pam);
        propmap[prop] = pam;
        qDebug("propmap modifier %lX", (quint64)pam->sampleModifier());
        //pam->sampleModifier()->set();
    }
    else
    {
        qWarning() << "No plug-in ";
        qWarning("%s", pam->getName().data());
        delete pam;
    }
    //delete pam;
    //pam = 0;
}

void WPSynthesizer::removeProperty(WPProperty prop)
{
    if (!propmap[prop])
    {
        propmap.erase(prop);
        return;
    }
    qDebug("delete property %s [%lf,%lf]", prop.getArg().data(), prop.getInterval().begin().getValue().toDouble(), prop.getInterval().end().getValue().toDouble());
    //delete[] propmap[prop]->sampleModifier()->getNotes();
    delete propmap[prop];
    //qDebug("propmap.size = %d", propmap.size());
    propmap.erase(prop);
    //qDebug("propmap.size = %d", propmap.size());
}

int WPSynthesizer::processTuningFreqAmp(double time, std::vector<double> &freq, std::vector<double> &amp)
{
    std::map<WPProperty, WPPropertyAndModifiers *>::iterator propmapiter;
    int tuned = 0;
    for (propmapiter = propmap.begin(); propmapiter != propmap.end(); propmapiter++)
        if ((*propmapiter).second->sampleModifier()->isTuning())
        {
            freq = (*propmapiter).second->sampleModifier()->modifyFreq(time, freq);
            tuned++;
        }
    if (tuned == 0)
    {
        tuned++;
        freq = defaulttuning.modifyFreq(time, freq);
    }
    /*
    for (propmapiter = propmap.begin(); propmapiter != propmap.end(); propmapiter++)
        if ((*propmapiter).second->sampleModifier()->isFreqModifier() &&
            !(*propmapiter).second->sampleModifier()->isTuning())
        {
            freq = (*propmapiter).second->sampleModifier()->modifyFreq(time, freq);
        }
    for (propmapiter = propmap.begin(); propmapiter != propmap.end(); propmapiter++)
        if ((*propmapiter).second->sampleModifier()->isAmpModifier())
        {
            amp = (*propmapiter).second->sampleModifier()->modifyAmp(time, amp);
            qDebug("amp modified by prop %s", (*propmapiter).first.getArg().data());
        }
        */
    int freqerr = sortAndApplyModifier(
        [](WPModifier *modifier)
        {
            return modifier->isFreqModifier() && !modifier->isTuning();
        },
        [](WPModifier *modifier)
        {
            return modifier->isFreqModifier();
        },
        [&freq, time](WPModifier *modifier)
        {
            freq = modifier->modifyFreq(time, freq);
        }
    );
    int amperr = sortAndApplyModifier(
        [](WPModifier *modifier)
        {
            return (bool)modifier->isAmpModifier();
        },
        [](WPModifier *modifier)
        {
            return modifier->isAmpModifier();
        },
        [&amp, time](WPModifier *modifier)
        {
            amp = modifier->modifyAmp(time, amp);
        }
    );
    //qDebug("amp = %lf", amp[0]);
    //TODO: process freqerr and amperr
    return tuned;
}

int WPSynthesizer::processNote(double time, double &notelength)
{
    double tmp;
    std::map<WPProperty, WPPropertyAndModifiers *>::iterator propmapiter;
    int notemodicnt = 0;
    notelength = -1.0;
    for (propmapiter = propmap.begin(); propmapiter != propmap.end(); propmapiter++)
        if ((*propmapiter).second->sampleModifier()->isNoteModifier())
        {
            //qDebug("using note modifier %s", (*propmapiter).first.getArg().data());
            tmp = (*propmapiter).second->sampleModifier()->modifyNote(time);
            if (tmp > 0)
                notelength = tmp;
            notemodicnt++;
        }
    if (notemodicnt == 0)
    {
        //qDebug("default note modifier used");
        notemodicnt++;
        notelength = defaultnotemodifier.modifyNote(time);
        return notemodicnt;
    }
    return notemodicnt;
}

int WPSynthesizer::processTempo(double time, double &tempo)
{
    int tempocnt = 0;
    /*
    std::map<WPProperty, WPPropertyAndModifiers *>::iterator propmapiter;
    for (propmapiter = propmap.begin(); propmapiter != propmap.end(); propmapiter++)
        if ((*propmapiter).second->sampleModifier()->isTempoModifier())
        {
            tempo = (*propmapiter).second->sampleModifier()->modifyTempo(time, tempo);
            tempocnt++;
        }
        */
    int tempoerr = sortAndApplyModifier(
        [](WPModifier *modifier)
        {
            return (bool)modifier->isTempoModifier();
        },
        [](WPModifier *modifier)
        {
            return modifier->isTempoModifier();
        },
        [&tempo, time, &tempocnt](WPModifier *modifier)
        {
            tempo = modifier->modifyTempo(time, tempo);
            tempocnt++;
        }
    );
    if (tempocnt == 0)
    {
        tempocnt++;
        tempo = 100.0; //100.0;
    }
    qDebug("##### process tempo = %lf", tempo);
    //TODO: process tempoerr
    return tempocnt;
}

int WPSynthesizer::processTimbre(double time, std::string &timbrename)
{
    int timbrecnt = 0;
    std::map<WPProperty, WPPropertyAndModifiers *>::iterator propmapiter;
    for (propmapiter = propmap.begin(); propmapiter != propmap.end(); propmapiter++)
        if ((*propmapiter).second->sampleModifier()->isTimbreModifier())
        {
            timbrename = (*propmapiter).second->sampleModifier()->modifyTimbre();
            timbrecnt++;
        }
    if (timbrecnt == 0)
    {
        timbrecnt++;
        timbrename = "WPTuningFork";
    }
    return timbrecnt;
}
/*
int WPSynthesizer::processTimbre(double time, WPPropertyAndModifiers *&timbreobj)
{
    int timbrecnt = 0;
    std::map<WPProperty, WPPropertyAndModifiers *>::iterator propmapiter;
    for (propmapiter = propmap.begin(); propmapiter != propmap.end(); propmapiter++)
        if ((*propmapiter).second->sampleModifier()->isTimbreModifier())
        {
            //timbrename = (*propmapiter).second->sampleModifier()->modifyTimbre();
            timbreobj = (*propmapiter).second;
            timbrecnt++;
        }
    if (timbrecnt == 0)
    {
        timbrecnt++;
        //timbrename = "WPTuningFork";
        timbreobj = ...;
    }
    return timbrecnt;
}
*/

//TODO: not tested
int WPSynthesizer::sortAndApplyModifier(bool (*filter)(WPModifier *), WPModifier::Precedence (*precedence)(WPModifier *), std::function<void (WPModifier *)> action)
{
    std::vector<WPModifier *> prec[WPModifier::PROP + 1];
    std::vector<WPModifier *>::iterator piter;
    int i;
    std::map<WPProperty, WPPropertyAndModifiers *>::iterator propmapiter;
    for (propmapiter = propmap.begin(); propmapiter != propmap.end(); propmapiter++)
        if (filter((*propmapiter).second->sampleModifier()))
        {
            prec[precedence((*propmapiter).second->sampleModifier())].push_back((*propmapiter).second->sampleModifier());
        }
    for (i = WPModifier::INIT; i <= WPModifier::PROP; i++)
        for (piter = prec[i].begin(); piter != prec[i].end(); piter++)
        {
            action(*piter);
        }
    return (prec[WPModifier::NEVER].size() <= 0 ? 0 : 1) |
           (prec[WPModifier::INIT].size() <= 1 ? 0 : 2) |
           (prec[WPModifier::DISPROP].size() <= 1 ? 0 : 4);
}

void WPSynthesizer::processAllModifiers(double time, std::vector<double> &freq, std::vector<double> &amp, double &notelength, double &tempo, std::string &timbrename)
{
    std::map<WPProperty, WPPropertyAndModifiers *>::iterator propmapiter;
    for (propmapiter = propmap.begin(); propmapiter != propmap.end(); propmapiter++)
    {
        double ttime = 0.0;
        double rtime = time - (*propmapiter).first.getInterval().begin().getValue().toDouble();/*
        //WPProperty prop = (*propmapiter).first;
        qDebug("Modifier: %lX", (quint64)(*propmapiter).second->sampleModifier());
        qDebug("Modifier: %lX", (quint64)(*propmapiter).second->sampleModifier());*/
        WPModifier *modifier = (*propmapiter).second->sampleModifier();
        //qDebug("relative time = %lf", rtime);
        /*qDebug("abcdefghijklmno %lf", ttime);
        qDebug("Modifier: %lX", (quint64)(*propmapiter).second->sampleModifier());
        qDebug("Modifier: %lX", (quint64)(*propmapiter).second->sampleModifier());
        qDebug("relative time = %lf", rtime);
        qDebug("Modifier: %lX", (quint64)(*propmapiter).second->sampleModifier());
        qDebug("Modifier: %lX", (quint64)modifier);*/
        modifier->setTime(rtime);
    }
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
    //qDebug("Final output");
    int j;
    timbremanager.clearTimbres();
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
    //qDebug("##### tempo = %lf %lf", tempo[0], tempo[1]);
    for (samplei = 1; samplei < samplecnt; samplei++)
    {
        vtime[samplei] = vtime[samplei - 1] + (time[samplei] - time[samplei - 1]) / ((tempo[samplei] + tempo[samplei - 1]) / 2.0 / 4.0 / 60.0);
        dur += vtime[samplei] - vtime[samplei - 1];
    }
    timbre.clear();
    for (j = 0; j < notes.size(); j++)
    {
        timbre.push_back(timbremanager.newTimbre());
        timbre[j]->reset();
    }
    //for (samplei = 1; samplei < samplecnt; samplei++)
    samplei = 1;
    while (samplei < samplecnt)
    {/*
        while (((WPPipe *)output)->isDefSuf() == 1)
        {
            //qDebug("Synthesizer %X waiting", (quint64)this);
            QThread::usleep(1000000);
        }*/
        swave->clear();
        for (j = 0; j < notes.size(); j++)
        {
            twave = timbre[j]->synthesize(dur, vtime[samplei - 1], vtime[samplei], amp[samplei - 1][j], amp[samplei][j], freq[samplei - 1][j], freq[samplei][j]);
            swave->mixWith(1.0, *twave, 1.0);
            delete twave;
            twave = 0;
        }
        if (output->write((char *)swave->data.begin(), swave->data.size() * sizeof(WPWave::WaveDataType)) == -1)
        {
            qCritical() << output->errorString();
        }
        ///
        total+=swave->data.size(); //* sizeof(WPWave::WaveDataType);
        //qDebug("Synthesizer %X written to output %d samples", (quint64)this, swave->data.size());
        //QThread::usleep(100); //remember to remove
        samplei++;
    }
    ///
    //qDebug("SYNTHESIZER %X: total=%d time=%lf", (quint64)this, total, time1);
}

void WPSynthesizer::outputNoteInitialize()
{
    int j;
    timbremanager.clearTimbres();
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
    for (samplei = 1; samplei < samplecnt; samplei++)
    {
        vtime[samplei] = vtime[samplei - 1] + (time[samplei] - time[samplei - 1]) / ((tempo[samplei] + tempo[samplei - 1]) / 2.0 / 4.0 / 60.0);
        dur += vtime[samplei] - vtime[samplei - 1];
    }
    timbre.clear();
    for (j = 0; j < notes.size(); j++)
    {
        timbre.push_back(timbremanager.newTimbre());
        timbre[j]->reset();
    }
    //for (samplei = 1; samplei < samplecnt; samplei++)
    samplei = 1;
    scheduleOutputNoteLoopBody();
}

void WPSynthesizer::scheduleOutputNoteLoopBody()
{
    outputtimer.singleShot(0, this, SLOT(outputNoteLoopBody()));
}

void WPSynthesizer::outputNoteLoopBody()
{
    if (samplei >= samplecnt)
        outputNoteFinalize();
    else
    {
        if (((WPPipe *)output)->isDefSuf() == 1)
        {
            qDebug("Synthesizer %X waiting", (quint64)this);
            outputtimer.singleShot(waitingtime, this, SLOT(outputNoteLoopBody()));
            return;
        }
        swave->clear();
        for (int j = 0; j < notes.size(); j++)
        {
            twave = timbre[j]->synthesize(dur, vtime[samplei - 1], vtime[samplei], amp[samplei - 1][j], amp[samplei][j], freq[samplei - 1][j], freq[samplei][j]);
            //timbre[j]->set2(Fraction(1, 2));
            swave->mixWith(1.0, *twave, 1.0);
            delete twave;
            twave = 0;
        }
        if (output->write((char *)swave->data.begin(), swave->data.size() * sizeof(WPWave::WaveDataType)) == -1)
        {
            qCritical() << output->errorString();
        }
        ///
        total+=swave->data.size(); //* sizeof(WPWave::WaveDataType);
        //qDebug("Synthesizer %X written to output %d samples", (quint64)this, swave->data.size());
        //QThread::usleep(100); //remember to remove
        samplei++;
        scheduleOutputNoteLoopBody();
    }
}

void WPSynthesizer::outputNoteFinalize()
{
    ///
    //qDebug("SYNTHESIZER %X: total=%d time=%lf", (quint64)this, total, time1);
    std::vector<double> efreq = freq[freq.size() - 1];
    freq.clear();
    freq.push_back(efreq);
    std::vector<double> eamp = amp[amp.size() - 1];
    amp.clear();
    amp.push_back(eamp);
    double etempo = tempo[tempo.size() - 1];
    tempo.clear();
    tempo.push_back(etempo);
    time.clear();
    time.push_back(time1);
    samplecnt = 1;

    time0 = time1;
    time1 = std::min(time1 + TimeStep, timeend);
    scheduleSynthesizePartLoopBody();
}

//static

WPWave::WaveDataType WPSynthesizer::truncateWaveData(double x)
{
    if (x > 1.0)
        x = 1.0;
    if (x < -1.0)
        x = -1.0;
    return WPWave::WaveDataType(x * 32767); //is 32767 safe?
}
