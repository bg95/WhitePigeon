#include "WPSynthesizer.h"

WPSynthesizer::WPSynthesizer(QObject *parent) :
    QObject(parent)
{
}

WPSynthesizer::WPSynthesizer(WPTimbre *_timbre, QObject *parent) :
    QObject(parent)
{
    loadTimbre(_timbre);
}

void WPSynthesizer::loadTimbre(const WPTimbre *_timbre)
{
    timbre = _timbre;
}

WPWave *WPSynthesizer::synthesize(WPNote &note)
{
    //return waveFunction(note.getFrequency(), note.getTimeSpan());
    double amp = 1.0, freq = note.getFrequency();
    return timbre->synthesize(note.getTimeSpan(), &amp, &freq); //take care of overflow
}

void WPSynthesizer::setBufferSize(quint32 size)
{
    buffersize = size;
}

quint32 WPSynthesizer::getBufferSize() const
{
    return buffersize;
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
