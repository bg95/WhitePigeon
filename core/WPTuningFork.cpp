#include "WPTuningFork.h"

WPTuningFork::WPTuningFork()
{
}

WPWave *WPTuningFork::synthesize(double dur, double *amp, double *freq) const
{
    QVector<WPWave::WaveDataType> tmpdata;
    QAudioFormat format = WPWave::defaultAudioFormat();
    int i;
    double t, phi;

    tmpdata.clear();
    phi = 0;
    for (i = 0; i < dur * format.sampleRate(); i++)
    {
        t = double(i) / double(format.sampleRate());
        tmpdata.push_back(WPSynthesizer::truncateWaveData(
            interpolate(amp, t) * std::min(std::exp(-1.0 * t), 50.0 * t) *
            std::sin(phi)
        ));
        phi += 2 * WPWave::PI * interpolate(freq, t) / double(format.sampleRate());
    }

    return new WPWave(tmpdata, format);
}
