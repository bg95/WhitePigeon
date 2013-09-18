#ifndef WPMIXER_H
#define WPMIXER_H

#include <QObject>
#include <QTimer>
#include "WPPipe.h"
#include "WPWave.h"

class WPMixer : public QObject
{
    Q_OBJECT
public:
    explicit WPMixer(QObject *parent = 0);
    ~WPMixer();

    bool openInputChannels(int number_of_channels);
    WPPipe *getInputChannel(int num) const;
    void setOutput(QIODevice &_output);
    void setWatingTime(int msec);
    void setReadLength(quint64 length);
    void start();
    
signals:
    void allInputClosed();
    
public slots:

private slots:
    void sumUp();

private:
    static void truncateAdd(WPWave::WaveDataType &a, WPWave::WaveDataType b);
    WPPipe *channel;
    int chcnt;
    quint64 readlength; //in number of samples
    QIODevice *output;
    QTimer timer;
    int waitingtime;
    WPWave::WaveDataType *sdata, *tdata;

};

#endif // WPMIXER_H
