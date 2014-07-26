#ifndef WPWAVE_H
#define WPWAVE_H

#include <complex>

#include <QObject>
#include <QVector>
#include <QString>
#include <QAudio>
#include <QAudioDecoder>
#include <QAudioBuffer>
#include <QAudioOutput>
#include <QBuffer>
#include <QFile>
#include <QMediaPlayer>

class WPWave : public QObject
{
    Q_OBJECT
public:
    typedef qint16 WaveDataType;
    static const WaveDataType maxWaveDataType, minWaveDataType;
    static const int SamplingRate;
    static const double PI;
    static const double Ksigma;   //The Gauss function is considered very small (as 0) if |x| > Ksigma * sigma
    //_FFT means Inverse FFT, so as _STFT, _Gabor
    template <class Type> static int FFT(const Type *begin, const Type *end, QVector<std::complex<double> > &FFTout);
    static int _FFT(const std::complex<double> *begin, const std::complex<double> *end, QVector<std::complex<double> > &FFTout);
    template <class Type> static void STFT(const Type *begin, const Type *end, double *window, int width, int period, QVector<QVector<std::complex<double> > > &out);
    static void _STFT(const QVector<QVector<std::complex<double> > > &in, double *window, int width, int period, QVector<std::complex<double> > &out);
    static double Gauss(double sigma, double x);
    template <class Type> static void Gabor(const Type *begin, const Type *end, double sigma, int period, QVector<QVector<std::complex<double> > > &out);
    static void _Gabor(const QVector<QVector<std::complex<double> > > &in, double sigma, int period, QVector<std::complex<double> > &out);
    static QAudioFormat defaultAudioFormat();

    static WPWave *newWPWave(const QVector<WaveDataType> &_data, const QAudioFormat &_format);

    explicit WPWave(QObject *parent = 0);
    WPWave(const QVector<WaveDataType> &_data, const QAudioFormat &_format, QObject *parent = 0);
    WPWave(const QByteArray &_data, const QAudioFormat &_format, QObject *parent = 0);
    WPWave(WPWave &b);

    void readFile(QString filename);
    void clear();
    void setData(const QVector<WaveDataType> &_data);
    void setData(const QByteArray &bytearray);
    void setFormat(const QAudioFormat &_format);
    const QVector<WPWave::WaveDataType> &getData() const;
    const QVector<std::complex<double> > &getFFTdata() const;
    const QVector<QVector<std::complex<double> > > &getSTFTdata() const;
    void FFT();
    void _FFT();
    void STFT(double *window, int width, int period);
    void _STFT(double *window, int width, int period);
    void Gabor(double sigma, int period);
    void _Gabor(double sigma, int period);
    void append(WPWave &b);
    void mixWith(double ra, const WPWave &b, double rb);
    void copy(WPWave &b);

    QVector<WaveDataType> data;
    QVector<std::complex<double> > FFTdata;
    QVector<QVector<std::complex<double> > > STFTdata;
    
signals:
    void finished();
    
public slots:
    bool play();
    void stop();
    
private:
    QAudioFormat format;
    QAudioDecoder *decoder;
    //bool isdecoded; //never used
    //bool isFFTed; //never used

    QByteArray *bytearray;
    QBuffer *buffer;
    QFile *bufferfile;
    QAudioOutput *audiooutput;
    QMediaPlayer *player;

private slots:
    void readBuffer();
    void decodeFinished();
    void audiooutputStateChanged(QAudio::State);

};

#endif // WPWAVE_H
