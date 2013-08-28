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
}

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
}
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
        iamp * (t > 0.9 * dur ? 0 : 1 - std::cos(2 * WPWave::PI * t / (0.9 * dur))) *
        std::sin(phi)
        ));
        phi += 2 * WPWave::PI * ifreq / double(WPWave::SamplingRate);
    }
    time = t;

    return WPWave_newWPWave(tmpdata, WPWave_defaultAudioFormat());
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
