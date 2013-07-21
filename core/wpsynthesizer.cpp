#include "wpsynthesizer.h"

WPSynthesizer::WPSynthesizer(QObject *parent) :
    QObject(parent)
{
}

WPSynthesizer::WPSynthesizer(TimbreType type, QString timbrename, QObject *parent) :
    QObject(parent)
{
    loadTimbre(type, timbrename);
}

void WPSynthesizer::loadTimbre(TimbreType type, QString timbrename)
{
    if (type == Internal)
    {
        if (timbrename == "Tuning Fork")
            waveFunction = waveTuningFork;
    }
}

WPWave *WPSynthesizer::synthesize(WPNote &note)
{
    return waveFunction(note.getFrequency(), note.getTimeSpan());
}

//static

WPWave::WaveDataType truncateWaveData(double x)
{
    if (x > 1.0)
        x = 1.0;
    if (x < -1.0)
        x = -1.0;
    return WPWave::WaveDataType(x * 32767);
}

WPWave *WPSynthesizer::waveTuningFork(double frequency, double duration)
{
    QVector<WPWave::WaveDataType> tmpdata;
    QAudioFormat format;
    int i;

    format.setChannelCount(1);
    format.setCodec("audio/x-raw");
    format.setSampleType(QAudioFormat::SignedInt);
    format.setSampleRate(48000);
    format.setSampleSize(8 * sizeof(WPWave::WaveDataType));

    tmpdata.clear();
    for (i = 0; i < duration * format.sampleRate(); i++)
    {
        double t = double(i) / double(format.sampleRate());
        tmpdata.push_back(truncateWaveData(std::exp(-1.0 * t) * std::sin(2 * WPWave::PI * frequency * t)));
    }

    return new WPWave(tmpdata, format);
}
