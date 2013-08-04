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
    //needs modification
    WPWave *synthesize(double dur, double *amp, double *freq) const
    {
          QVector<WPWave::WaveDataType> tmpdata;
          QAudioFormat format = WPWave::defaultAudioFormat();
          int i;

          tmpdata.clear();
          for (i = 0; i < dur * format.sampleRate(); i++)
          {
              double t = double(i) / double(format.sampleRate());
              tmpdata.push_back(WPSynthesizer::truncateWaveData(*amp * std::exp(-1.0 * t) * std::sin(2 * WPWave::PI * *freq * t)));
          }

          return new WPWave(tmpdata, format);
    }
};

#endif // WPTUNINGFORK_H
