#include "WPTuningFork.h"

void *(*WPTuningFork::callback)(WPCallbackManager::CallbackFunc) = 0;

WPTuningFork::WPTuningFork()
{
	printf("WPTuningFork constructor\n");
	fflush(stdout);
}

WPTuningFork::WPTuningFork(int i)
{
}

WPTuningFork::WPTuningFork(double d)
{
}

WPTuningFork::~WPTuningFork()
{
	printf("WPTuningFork destructor\n");
	fflush(stdout);
}

void WPTuningFork::reset()
{
	time = 0;
	phi = 0;
	phi2 = 0;
}
/*
WPWave *WPTuningFork::synthesize(double dur, double *amp, double *freq) const
{
    QVector<WPWave::WaveDataType> tmpdata;
    QAudioFormat (*WPWave_defaultAudioFormat)() = (QAudioFormat (*)())callback(WPCallbackManager::WPWave_defaultAudioFormat);
    WPWave::WaveDataType (*WPSynthesizer_truncateWaveData)(double) = (WPWave::WaveDataType (*)(double))callback(WPCallbackManager::WPSynthesizer_truncateWaveData);
    WPWave *(*WPWave_newWPWave)(const QVector<WPWave::WaveDataType> &, const QAudioFormat &) =
    	(WPWave *(*)(const QVector<WPWave::WaveDataType> &, const QAudioFormat &))callback(WPCallbackManager::WPWave_newWPWave);
    int i;
    double t, phi;

    tmpdata.clear();
    phi = 0;
    for (i = 0; i < dur * WPWave::SamplingRate; i++)
    {
        t = double(i) / double(WPWave::SamplingRate);
        tmpdata.push_back(WPSynthesizer_truncateWaveData(
        interpolate(amp, t) * (t > 0.9 * dur ? 0 : 1 - std::cos(2 * WPWave::PI * t / (0.9 * dur))) *
        std::sin(phi)
        ));
        phi += 2 * WPWave::PI * interpolate(freq, t) / double(WPWave::SamplingRate);
    }

    return WPWave_newWPWave(tmpdata, WPWave_defaultAudioFormat());
}*/
WPWave *WPTuningFork::synthesize(double dur, double time0, double time1, double amp0, double amp1, double freq0, double freq1)
{
    QVector<WPWave::WaveDataType> tmpdata;
    QAudioFormat (*WPWave_defaultAudioFormat)() = (QAudioFormat (*)())callback(WPCallbackManager::WPWave_defaultAudioFormat);
    WPWave::WaveDataType (*WPSynthesizer_truncateWaveData)(double) = (WPWave::WaveDataType (*)(double))callback(WPCallbackManager::WPSynthesizer_truncateWaveData);
    WPWave *(*WPWave_newWPWave)(const QVector<WPWave::WaveDataType> &, const QAudioFormat &) =
    	(WPWave *(*)(const QVector<WPWave::WaveDataType> &, const QAudioFormat &))callback(WPCallbackManager::WPWave_newWPWave);
    int i;
    double t, iamp, ifreq;

    tmpdata.clear();
    for (t = time; t < time1; t += 1.0 / double(WPWave::SamplingRate))
    {
    	iamp = interpolate(t, time0, time1, amp0, amp1);
    	ifreq = interpolate(t, time0, time1, freq0, freq1);
        tmpdata.push_back(WPSynthesizer_truncateWaveData(
        iamp * 0.1 * //envelope(0.1 * dur, 0.8 * dur, 0.1 * dur, t) * 0.25 *
        (
        	std::sin(phi) * std::exp(-1.0 * t) +/*
            std::sin(phi * 2.0) * std::exp(-4.0 * t) +
        	std::sin(phi * 3.0) * std::exp(-9.0 * t) +
        	std::sin(phi * 4.0) * std::exp(-16.0 * t) +*/
            std::sin(phi * 5.0) * std::exp(-25.0 * t) +
            std::sin(phi * 9.0) * std::exp(-81.0 * t)
        )
        ));
        phi += 2 * WPWave::PI * ifreq / double(WPWave::SamplingRate);
    }
    time = t;

    return WPWave_newWPWave(tmpdata, WPWave_defaultAudioFormat());
}

double WPTuningFork::envelope(double rise, double sustain, double decay, double t)
{
	if (t < 0)
		return 0.0;
	if (0 <= t < rise)
		return std::exp(t / rise * 12.0 - 12.0);
	if (t >= rise && t < rise + sustain)
		return 1.0;
	t -= rise + sustain;
	if (t < decay)
		return std::exp(-t / decay * 12.0);
	return 0.0;
}

double WPTuningFork::rand11()
{
	return (double)rand() / (double)RAND_MAX * 2.0 - 1.0;
}

extern "C"
{
	WPTimbre *create()
	{
		return new WPTuningFork();
	}
	void destroy(WPTimbre *p)
	{
		printf("destroy called\n");
		fflush(stdout);
		if (p)
			delete p;
		p = 0;
	}
	void setCallback(void *_callback)
    {
        WPTuningFork::callback = (void *(*)(WPCallbackManager::CallbackFunc))_callback;
    }
}
