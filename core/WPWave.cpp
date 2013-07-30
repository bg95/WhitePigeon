#include <stdio.h>
#include "WPWave.h"

WPWave::WPWave(QObject *parent) :
    QObject(parent)
{
    isdecoded = false;
    isFFTed = false;
}

WPWave::WPWave(const QVector<WaveDataType> &_data, const QAudioFormat &_format, QObject *parent) :
    QObject(parent),
    format(_format)
{
    setData(_data);
}

WPWave::WPWave(const QByteArray &bytearray, const QAudioFormat &_format, QObject *parent) :
    QObject(parent),
    format(_format)
{
    setData(bytearray);
}

WPWave::WPWave(WPWave &b) :
    QObject(b.parent()),
    data(b.data),
    FFTdata(b.FFTdata),
    STFTdata(b.STFTdata),
    format(b.format),
    decoder(),
    isdecoded(b.isdecoded),
    isFFTed(b.isFFTed)
{
}

void WPWave::readFile(QString filename)
{
    isdecoded = false;
    data.clear();
/*
    format.setChannelCount(1);
    format.setCodec("audio/x-raw");
    format.setSampleType(QAudioFormat::SignedInt);
    format.setSampleRate(48000);
    format.setSampleSize(8 * sizeof(WaveDataType));
*/
    format = defaultAudioFormat();
    decoder.setAudioFormat(format);
    decoder.setSourceFilename(filename);

    connect(&decoder, SIGNAL(bufferReady()), this, SLOT(readBuffer()));
    connect(&decoder, SIGNAL(finished()), this, SLOT(decodeFinished()));
    decoder.start();
}

void WPWave::setData(const QVector<WaveDataType> &_data)
{
    data = _data;
    isdecoded = true;
    isFFTed = false;
}

void WPWave::setData(const QByteArray &bytearray)
{
    quint64 length = bytearray.size() / sizeof(WaveDataType);
    WaveDataType *begin = (WPWave::WaveDataType *)bytearray.constData();
    data.clear();
    for (quint64 i = 0; i < length; i++)
    {
        data.push_back(*begin);
        begin++;
    }
}

void WPWave::setFormat(const QAudioFormat &_format)
{
    format = _format;
}

const QVector<WPWave::WaveDataType> &WPWave::getData() const
{
    return data;
}

const QVector<std::complex<double> > &WPWave::getFFTdata() const
{
    return FFTdata;
}

const QVector<QVector<std::complex<double> > > &WPWave::getSTFTdata() const
{
    return STFTdata;
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
        data[i] = auxdata[i].real(); //?
}

void WPWave::STFT(double *window, int width, int period)
{
    STFT(data.begin(), data.end(), window, width, period, STFTdata);
}
void WPWave::_STFT(double *window, int width, int period)
{
    QVector<std::complex<double> > auxdata;
    _STFT(STFTdata, window, width, period, auxdata);
    int i;
    for (i = 0; i < data.size(); i++)
        data[i] = auxdata[i].real(); //?
}

void WPWave::Gabor(double sigma, int period)
{
    Gabor(data.begin(), data.end(), sigma, period, STFTdata);
}
void WPWave::_Gabor(double sigma, int period)
{
    QVector<std::complex<double> > auxdata;
    _Gabor(STFTdata, sigma, period, auxdata);
    int i;
    for (i = 0; i < data.size(); i++)
        data[i] = auxdata[i].real(); //?
}

void WPWave::append(WPWave &b)
{
    int i;
    for (i = 0; i < b.data.size(); i++)
        data.push_back(b.data[i]);
    isFFTed = false;
}

void WPWave::mixWith(double ra, const WPWave &b, double rb)
{
    int i, n;
    n = data.size();
    if (b.data.size() > n)
    {
        for (i = 0; i < n; i++)
            data[i] = data[i] * ra + b.data[i] * rb;
        for (i = n; i < b.data.size(); i++)
            data[i] = b.data[i] * rb;
    }
    else
    {
        for (i = 0; i < b.data.size(); i++)
            data[i] = data[i] * ra + b.data[i] * rb;
        for (i = b.data.size(); i < n; i++)
            data[i] = data[i] * ra;
    }
}

void WPWave::copy(WPWave &b)
{
    data = b.data;
    FFTdata = b.FFTdata;
    STFTdata = b.STFTdata;
    format = b.format;
    isdecoded = b.isdecoded;
    isFFTed = b.isFFTed;
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
    std::complex<double> omega, pomega, temp;
    for (n2 = 1; n2 < n; n2 <<= 1);
    FFTout.clear();
    FFTout.resize(n2);/*
    for (i = 0; i < n; i++)
        FFTout.push_back(std::complex<double>(begin[i]));
    for (i = n; i < n2; i++)
        FFTout.push_back(std::complex<double>(0));*/
    for (i = 0; i < n; i++)
        FFTout[i] = std::complex<double>(begin[i]);
    for (i = n; i < n2; i++)
        FFTout[i] = std::complex<double>(0);

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

    for (i = 2; i <= n2; i <<= 1)
    {
        omega = std::complex<double>(cos(2 * PI / i), sin(2 * PI / i));
        pomega = 1.0;
        for (k = 0; k < i / 2; k++)
        {
            for (j = 0; j < n2; j += i)
            {
                temp = pomega * FFTout[j + k + i / 2];
                FFTout[j + k] = FFTout[j + k] + temp;
                FFTout[j + k + i / 2] = FFTout[j + k] - temp - temp;
            }
            pomega *= omega;
        }
    }
    return n2;
}

int WPWave::_FFT(const std::complex<double> *begin, const std::complex<double> *end, QVector<std::complex<double> > &FFTout)
{
    int n = end - begin;
    int n2;
    int i, j, k;
    std::complex<double> omega, pomega, temp;
    for (n2 = 1; n2 < n; n2 <<= 1);
    FFTout.clear();
    FFTout.resize(n2);/*
    for (i = 0; i < n; i++)
        FFTout.push_back(std::complex<double>(begin[i]));
    for (i = n; i < n2; i++)
        FFTout.push_back(std::complex<double>(0));*/
    for (i = 0; i < n; i++)
        FFTout[i] = std::complex<double>(begin[i]);
    for (i = n; i < n2; i++)
        FFTout[i] = std::complex<double>(0);

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

    for (i = 2; i <= n2; i <<= 1)
    {
        omega = std::complex<double>(cos(2 * PI / i), -sin(2 * PI / i));
        pomega = 1.0;
        for (k = 0; k < i / 2; k++)
        {
            for (j = 0; j < n2; j += i)
            {
                temp = pomega * FFTout[j + k + i / 2];
                FFTout[j + k] = FFTout[j + k] + temp;
                FFTout[j + k + i / 2] = FFTout[j + k] - temp - temp;
            }
            pomega *= omega;
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
void WPWave::STFT(const Type *begin, const Type *end, double *window, int width, int period, QVector<QVector<std::complex<double> > > &out)
{
    double *tmpwave;
    int mid, i, k, n;
    n = end - begin;
    tmpwave = new double[width * 2 + 1];
    out.clear();
    printf("STFT: width = %d\n", width);
    k = 0;
    for (mid = 0; mid < n; mid += period)
    {
        for (i = -width; i <= width; i++)
        {
            if (mid + i >= 0 && mid + i < n)
                tmpwave[i + width] = begin[mid + i] * window[std::abs(i)];
            else
                tmpwave[i + width] = 0;
        }
        out.push_back(QVector<std::complex<double> >());
        FFT(tmpwave, tmpwave + width * 2 + 1, out[k]);
        k++;
    }
}

void WPWave::_STFT(const QVector<QVector<std::complex<double> > > &in, double *window, int width, int period, QVector<std::complex<double> > &out)
{
    int n = in.size() * period;
    int i, k, mid;
    QVector<std::complex<double> > tmpwave;
    double *scale;
    scale = new double[n];
    out.clear();
    out.fill(0.0, n);
    for (i = 0; i < n; i++)
        scale[i] = 0.0;
    printf("_STFT: width = %d\n", width);
    k = 0;
    for (mid = 0; mid < n; mid += period)
    {
        _FFT(in[k].begin(), in[k].end(), tmpwave);/*
        printf("in[%d].size() == %d\n", k, in[k].size());
        Q_ASSERT_X(in[k].size() >= width * 2 + 1, "_STFT", "in[k] size error");
        Q_ASSERT_X(tmpwave.size() >= width * 2 + 1, "_STFT", "tmpwave size error");*/
        for (i = -width; i <= width; i++)
        {
            if (mid + i >= 0 && mid + i < n)
            {
                out[mid + i] += tmpwave[i + width];
                scale[mid + i] += window[std::abs(i)];
            }
        }
        k++;
    }
    for (i = 0; i < n; i++)
        out[i] /= scale[i];
    delete[] scale;
}

template <class Type>
void WPWave::Gabor(const Type *begin, const Type *end, double sigma, int period, QVector<QVector<std::complex<double> > > &out)
{
    double *gausswindow;
    int width = int(Ksigma * sigma + 1.0);
    int i;
    gausswindow = new double[width + 1];
    for (i = 0; i <= width; i++)
        gausswindow[i] = Gauss(sigma, i);
    STFT(begin, end, gausswindow, int(Ksigma * sigma + 1.0), period, out);
    delete[] gausswindow;
}

void WPWave::_Gabor(const QVector<QVector<std::complex<double> > > &in, double sigma, int period, QVector<std::complex<double> > &out)
{
    double *gausswindow;
    int width = int(Ksigma * sigma + 1.0);
    int i;
    gausswindow = new double[width + 1];
    for (i = 0; i <= width; i++)
        gausswindow[i] = Gauss(sigma, i);
    _STFT(in, gausswindow, width, period, out);
    delete[] gausswindow;
}

QAudioFormat WPWave::defaultAudioFormat()
{
    QAudioFormat f;
    f.setChannelCount(1);
    f.setCodec("audio/x-raw");
    f.setSampleType(QAudioFormat::SignedInt);
    f.setSampleRate(48000);
    f.setSampleSize(8 * sizeof(WaveDataType));
    return f;
}
