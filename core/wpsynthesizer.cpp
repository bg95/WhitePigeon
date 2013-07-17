#include "wpsynthesizer.h"

WPSynthesizer::WPSynthesizer(QObject *parent) :
    QObject(parent)
{
}

//static

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
        tmpdata.push_back(std::exp(-1.0 * t) * std::sin(2 * WPWave::PI * frequency * t));
    }

    return new WPWave(tmpdata, format);
}
