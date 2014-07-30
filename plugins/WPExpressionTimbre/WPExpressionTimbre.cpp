#include "WPExpressionTimbre.h"

WPExpressionTimbre::WPExpressionTimbre()
{
    //printf("WPExpressionTimbre constructor\n");
    //fflush(stdout);
}

WPExpressionTimbre::~WPExpressionTimbre()
{
    //printf("WPExpressionTimbre destructor\n");
    //fflush(stdout);
}

void WPExpressionTimbre::reset()
{
	time = 0;
	phi = 0;
	phi2 = 0;
}

void WPExpressionTimbre::set(std::string para)
{
	expression = para;
}

WPWave *WPExpressionTimbre::synthesize(double dur, double time0, double time1, double amp0, double amp1, double freq0, double freq1)
{
    QVector<WPWave::WaveDataType> tmpdata;
	/*
    QAudioFormat (*WPWave_defaultAudioFormat)() = (QAudioFormat (*)())callback(WPCallbackManager::WPWave_defaultAudioFormat);
    WPWave::WaveDataType (*WPSynthesizer_truncateWaveData)(double) = (WPWave::WaveDataType (*)(double))callback(WPCallbackManager::WPSynthesizer_truncateWaveData);
    WPWave *(*WPWave_newWPWave)(const QVector<WPWave::WaveDataType> &, const QAudioFormat &) =
    	(WPWave *(*)(const QVector<WPWave::WaveDataType> &, const QAudioFormat &))callback(WPCallbackManager::WPWave_newWPWave);
		*/
    int i;
    double t, iamp, ifreq;

    tmpdata.clear();
    for (t = time; t < time1; t += 1.0 / double(WPWave::SamplingRate))
    {
    	iamp = interpolate(t, time0, time1, amp0, amp1);
    	ifreq = interpolate(t, time0, time1, freq0, freq1);
		calculator.setConstant("time", t);
		calculator.setConstant("amp", iamp);
		calculator.setConstant("freq", ifreq);
		calculator.setConstant("phi", phi);
        tmpdata.push_back(WPSynthesizer::truncateWaveData(
			calculator.evaluate(expression)
        ));
        phi += 2 * WPWave::PI * ifreq / double(WPWave::SamplingRate);
    }
    time = t;

    return WPWave::newWPWave(tmpdata, WPWave::defaultAudioFormat());
}
/*
double WPExpressionTimbre::envelope(double rise, double sustain, double decay, double t)
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
*/
double WPExpressionTimbre::rand11()
{
	return (double)rand() / (double)RAND_MAX * 2.0 - 1.0;
}

extern "C"
{
	WPTimbre *create()
	{
		return new WPExpressionTimbre();
	}
	void destroy(WPTimbre *p)
	{
        //printf("destroy called\n");
        //fflush(stdout);
		if (p)
			delete p;
		p = 0;
	}
}
