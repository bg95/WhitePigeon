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

typedef qint16 WaveDataType;
class WPWave : public QObject
{
    Q_OBJECT
public:
    static const double PI = 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679;
    static const double Ksigma = 6.0;   //The Gauss function is considered very small (as 0) if |x| > Ksigma * sigma
    template <class Type> static int FFT(const Type *begin, const Type *end, QVector<std::complex<double> > &FFTout);
    static int _FFT(const std::complex<double> *begin, const std::complex<double> *end, QVector<std::complex<double> > &FFTout);
    template <class Type> static void STFT(const Type *begin, const Type *end, double *window, int width, int period, QVector<QVector<std::complex<double> > > &out);
    static void _STFT(const QVector<QVector<std::complex<double> > > &in, double *window, int width, int period, QVector<std::complex<double> > &out);
    static double Gauss(double sigma, double x);
    template <class Type> static void Gabor(const Type *begin, const Type *end, double sigma, int period, QVector<QVector<std::complex<double> > > &out);
    static void _Gabor(const QVector<QVector<std::complex<double> > > &in, double sigma, int period, QVector<std::complex<double> > &out);

    explicit WPWave(QObject *parent = 0);
    WPWave(QVector<WaveDataType> &_data, QObject *parent = 0);
    void readFile(QString filename);
    void FFT();
    void _FFT();
    void Gabor(double sigma, int period);
    void _Gabor(double sigma, int period);

    QVector<WaveDataType> data;
    QVector<std::complex<double> > FFTdata;
    QVector<QVector<std::complex<double> > > STFTdata;
    
signals:
    void finished();
    
public slots:
    void readBuffer();
    void decodeFinished();
    bool play();
    void stop();
    void audiooutputStateChanged(QAudio::State);
    
private:
    QAudioFormat format;
    QAudioDecoder decoder;
    bool isdecoded;
    bool isFFTed;

    QByteArray *bytearray;
    QBuffer *buffer;
    QFile *bufferfile;
    QAudioOutput *audiooutput;
    QMediaPlayer *player;

};

#endif // WPWAVE_H
