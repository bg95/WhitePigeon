#include <stdio.h>
#include "wpwave.h"

WPWave::WPWave(QObject *parent) :
    QObject(parent)
{
    isdecoded = false;
    isFFTed = false;
}

WPWave::WPWave(QVector<WaveDataType> &_data, QObject *parent) :
    QObject(parent), data(_data)
{
    isdecoded = true;
    isFFTed = false;
}

void WPWave::readFile(QString filename)
{
    isdecoded = false;
    data.clear();

    format.setChannelCount(1);
    format.setCodec("audio/x-raw");
    format.setSampleType(QAudioFormat::SignedInt);
    format.setSampleRate(48000);
    format.setSampleSize(8 * sizeof(WaveDataType));

    decoder.setAudioFormat(format);
    decoder.setSourceFilename(filename);

    connect(&decoder, SIGNAL(bufferReady()), this, SLOT(readBuffer()));
    connect(&decoder, SIGNAL(finished()), this, SLOT(decodeFinished()));
    decoder.start();
}

void WPWave::FFT()
{
    FFT(data.begin(), data.end(), FFTdata);
}
void WPWave::_FFT()
{
    QVector<std::complex<double> > auxdata;
    _FFT(FFTdata.begin(), FFTdata.end(), auxdata);
    int i;
    for (i = 0; i < data.size(); i++)
        data[i] = auxdata[i].real()/* + auxdata[i].imag()*/; //?
}

bool WPWave::play()
{
    if (!isdecoded)
        return false;
    bytearray = new QByteArray((const char *)data.begin(), data.size() * format.sampleSize() / 8);
    buffer = new QBuffer(bytearray, this);
    buffer->open(QIODevice::ReadOnly);

    audiooutput = new QAudioOutput(format, this);
    connect(audiooutput, SIGNAL(stateChanged(QAudio::State)), this, SLOT(audiooutputStateChanged(QAudio::State)));
    audiooutput->start(buffer);
    printf("playing\n");
    fflush(stdout);
    return true;
}

void WPWave::stop()
{
    printf("stopped %d\n", audiooutput->state());
    qDebug() << "stopped " << audiooutput->state() << endl;
    audiooutput->stop();
    delete bytearray;
    delete buffer;
    delete audiooutput;
}

void WPWave::audiooutputStateChanged(QAudio::State state)
{
    switch (state)
    {
    case QAudio::IdleState:
        stop();
        break;
    case QAudio::StoppedState:
        if (audiooutput->error() != QAudio::NoError)
            printf("error!");
        break;
    }
}

void WPWave::readBuffer()
{
    QAudioBuffer audiobuffer = decoder.read();
    int n = audiobuffer.sampleCount();
    WaveDataType *buf = audiobuffer.data<WaveDataType>();
    int i;
    for (i = 0; i < n; i++)
        data.push_back(buf[i]);
}

void WPWave::decodeFinished()
{
    isdecoded = true;
    finished();
}

//static

template <class Type>
int WPWave::FFT(const Type *begin, const Type *end, QVector<std::complex<double> > &FFTout)
{
    int n = end - begin;
    int n2;
    int i, j, k;
    std::complex<double> omega, pomega;
    for (n2 = 1; n2 < n; n2 <<= 1);
    FFTout.clear();
    for (i = 0; i < n; i++)
        FFTout.push_back(std::complex<double>(begin[i]));
    for (i = n; i < n2; i++)
        FFTout.push_back(std::complex<double>(0));

    for (i = 0; i < n2; i++)
    {
        j = 0;
        for (k = 0; (1 << k) < n2; k++)
        {
            j = (j << 1) | ((i >> k) & 1);
        }
        if (j > i)
            std::swap(FFTout[i], FFTout[j]);
    }

    printf("FFT 0\n");

    for (i = 2; i <= n2; i <<= 1)
    {
        printf("FFT %d\n", i);
        omega = std::complex<double>(cos(2 * PI / i), sin(2 * PI / i));
        for (j = 0; j < n2; j += i)
        {
            pomega = 1.0;
            for (k = 0; k < i / 2; k++)
            {
                FFTout[j + k] = FFTout[j + k] + pomega * FFTout[j + k + i / 2];
                FFTout[j + k + i / 2] = FFTout[j + k] - 2.0 * pomega * FFTout[j + k + i / 2];
                pomega *= omega;
            }
        }
    }
    return n2;
}

int WPWave::_FFT(const std::complex<double> *begin, const std::complex<double> *end, QVector<std::complex<double> > &FFTout)
{
    int n = end - begin;
    int n2;
    int i, j, k;
    std::complex<double> omega, pomega;
    for (n2 = 1; n2 < n; n2 <<= 1);
    FFTout.clear();
    for (i = 0; i < n; i++)
        FFTout.push_back(std::complex<double>(begin[i]));
    for (i = n; i < n2; i++)
        FFTout.push_back(std::complex<double>(0));

    for (i = 0; i < n2; i++)
    {
        j = 0;
        for (k = 0; (1 << k) < n2; k++)
        {
            j = (j << 1) | ((i >> k) & 1);
        }
        if (j > i)
            std::swap(FFTout[i], FFTout[j]);
    }

    printf("FFT 0\n");

    for (i = 2; i <= n2; i <<= 1)
    {
        printf("FFT %d\n", i);
        omega = std::complex<double>(cos(2 * PI / i), -sin(2 * PI / i));
        for (j = 0; j < n2; j += i)
        {
            pomega = 1.0;
            for (k = 0; k < i / 2; k++)
            {
                FFTout[j + k] = FFTout[j + k] + pomega * FFTout[j + k + i / 2];
                FFTout[j + k + i / 2] = FFTout[j + k] - 2.0 * pomega * FFTout[j + k + i / 2];
                pomega *= omega;
            }
        }
    }
    for (i = 0; i < FFTout.size(); i++)
        FFTout[i] /= n2;
    return n2;
}

double WPWave::Gauss(double sigma, double x)
{
    return std::exp(-x * x / (sigma * sigma)) / (sigma * std::sqrt(PI));
    //very small if |x| > Ksigma * sigma
}

template <class Type>
void WPWave::Gabor(const Type *begin, const Type *end, double sigma, int period, QVector<QVector<std::complex<double> > > &out)
{
    double *tmpwave;
    int mid, i, k, n, width;
    n = end - begin;
    width = int(Ksigma * sigma + 1.0);
    tmpwave = new double[width * 2 + 1];
    out.clear();
    k = 0;
    for (mid = 0; mid < n; mid += period)
    {
        for (i = -width; i <= width; i++)
        {
            if (mid + i >= 0 && mid + i < n)
                tmpwave[i + width] = begin[mid + i] * Gauss(sigma, i);
            else
                tmpwave[i + width] = 0;
        }
        out.push_back(QVector<std::complex<double> >());
        FFT(tmpwave, tmpwave + width * 2 + 1, out[k]);
        k++;
    }
}

void WPWave::_Gabor(QVector<QVector<std::complex<double> > > &in, double sigma, int period, QVector<std::complex<double> > &out);
