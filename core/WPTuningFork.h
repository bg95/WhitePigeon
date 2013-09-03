#ifndef WPTUNINGFORK_H
#define WPTUNINGFORK_H

#include "WPTimbre.h"
#include "WPSynthesizer.h"

class WPTuningFork : public WPTimbre
{
public:
    //const static WPTuningFork TuningFork;

    WPTuningFork()
    {}
    WPWave *synthesize(double dur, double *amp, double *freq) const
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
                interpolate(amp, t) * std::exp(-1.0 * t) *
                std::sin(phi)
            ));
            phi += 2 * WPWave::PI * interpolate(freq, t) / double(format.sampleRate());
        }

        return new WPWave(tmpdata, format);
    }
};

#endif // WPTUNINGFORK_H
