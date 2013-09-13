#ifndef WPMIXER_H
#define WPMIXER_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include "WPPipe.h"
#include "WPWave.h"

class WPMixer : public QThread
{
    Q_OBJECT
public:
    explicit WPMixer(QObject *parent = 0);
    ~WPMixer();

    bool openInputChannels(int number_of_channels);
    WPPipe *getInputChannel(int num) const;
    void setOutput(QIODevice &_output);
    void setWaitingTime(int msec);
    void setReadLength(quint64 length);
    
signals:
    void allInputClosed();
    
public slots:

protected:
    void run();

private slots:
    void sumUp();

private:
    static void truncateAdd(WPWave::WaveDataType &a, WPWave::WaveDataType b);
    WPPipe *channel;
    quint32 chcnt;
    quint64 readlength; //in number of samples
    QIODevice *output;
    QTimer timer;
    int waitingtime;
    WPWave::WaveDataType *sdata, *tdata;
    //debug
    //QFile *filein, *fileout, *fileout2;

};

#endif // WPMIXER_H
