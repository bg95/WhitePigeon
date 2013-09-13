#include "WPMixer.h"

WPMixer::WPMixer(QObject *parent) :
    QThread(parent),
    channel(0),
    chcnt(0),
    output(0)
{
    timer.setSingleShot(true);
    connect(&timer, SIGNAL(timeout()), this, SLOT(sumUp()));
    sdata = 0;
    tdata = 0;
    setReadLength(1);
    /*//debug
    filein = 0;
    fileout = 0;
    fileout2 = 0;*/
}

WPMixer::~WPMixer()
{
    qDebug("Deleting mixer %X", (quint64)this);
    /*
    if (channel)
        delete[] channel;
    if (sdata != 0)
    {
        delete[] sdata;
        sdata = 0;
    }
    if (tdata != 0)
    {
        delete[] tdata;
        tdata = 0;
    }*/
    //I don't know if deleting is necessary
    quit();
    qDebug("Mixer %X waiting to stop", (quint64)this);
    wait();
}

bool WPMixer::openInputChannels(int number_of_channels)
{
    chcnt = number_of_channels;
    channel = new WPPipe[chcnt];
    quint32 i;
    for (i = 0; i < chcnt; i++)
        if (channel[i].open(QIODevice::ReadWrite) == false)
        {
            delete[] channel;
            return false;
        }
    return true;
}

WPPipe *WPMixer::getInputChannel(int num) const
{
    if (num < 0 || num >= chcnt)
        return 0;
    return channel + num;
}

void WPMixer::setOutput(QIODevice &_output)
{
    output = &_output;
}

void WPMixer::setWaitingTime(int msec)
{
    waitingtime = msec;
}

void WPMixer::setReadLength(quint64 length)
{
    readlength = length;
    if (sdata != 0)
    {
        delete[] sdata;
        sdata = 0;
    }
    if (tdata != 0)
    {
        delete[] tdata;
        tdata = 0;
    }
    sdata = new WPWave::WaveDataType[readlength];
    tdata = new WPWave::WaveDataType[readlength];
    //sdata = (WPWave::WaveDataType *)malloc(sizeof(WPWave::WaveDataType) * readlength);
    //tdata = (WPWave::WaveDataType *)malloc(sizeof(WPWave::WaveDataType) * readlength);
}

void WPMixer::run()
{
    if (chcnt == 0)
    {
        qCritical("Input channels not opened!");
        return;
    }
/*
    char filename[256];
    //sprintf(filename, "%llX.in", (quint64)this);
    sprintf(filename, "mixer.in");
    filein = new QFile(filename);
    qDebug("filein.open %d", filein->open(QIODevice::WriteOnly));
    //sprintf(filename, "%llX.out", (quint64)this);
    sprintf(filename, "mixer.out");
    fileout = new QFile(filename);
    qDebug("fileout.open %d", fileout->open(QIODevice::WriteOnly));
*/
    qDebug("Mixer %X in thread %X\n", (quint64)this, (quint64)QThread::currentThread());
    timer.start(0);
    exec();
}

void WPMixer::sumUp()
{
    //assume no other threads are reading the channel
    //while (true)
    //{
    qDebug("Mixer %X running in thread %X", (quint64)this, (quint64)QThread::currentThread());
    qint32 i, j;
    qint64 bytesavailable, maxbytesread, bytesread;
    qint64 readlengthbytes = readlength * sizeof(WPWave::WaveDataType);
    bool existopen;
    qDebug("mixer sumUp()");
    existopen = false;
    for (i = 0; i < chcnt; i++)
    {
        if (channel[i].isOpen())
            existopen = true;
        if (channel[i].isOpen() && !channel[i].isClosing() &&
            channel[i].bytesAvailable() < readlengthbytes)
            break;
    }
    if (i < chcnt)
    {
        qDebug("mixer %X waiting", (quint64)this);
        timer.start(waitingtime);
        return;
    }
    if (!existopen)
    {
        delete[] channel;
        chcnt = 0;
        emit allInputClosed();
        quit(); //?
        return;
    }
    memset(sdata, 0, readlengthbytes);
    maxbytesread = 0;
    for (i = 0; i < chcnt; i++)
    {
        bytesread = 0;
        if (channel[i].isOpen())
        {
            bytesavailable = channel[i].bytesAvailable();
            if (bytesavailable >= readlengthbytes)
            {
                //static int total=0;
                bytesread = channel[i].read((char *)tdata, readlengthbytes);
                //total+=bytesread;
                //qDebug("MIXER: total=%d",total);
                if (maxbytesread < bytesread)
                    maxbytesread = bytesread;
                for (j = 0; j < readlength; j++)
                    truncateAdd(sdata[j], tdata[j]);
            }
            else
            {
                qDebug("channel %d is closing and has only %llu bytes available", i, bytesavailable);
                bytesread = channel[i].read((char *)tdata, bytesavailable);
                if (maxbytesread < bytesread)
                    maxbytesread = bytesread;
                for (j = 0; j < bytesavailable / sizeof(WPWave::WaveDataType); j++)
                    truncateAdd(sdata[j], tdata[j]);
            }
        }
        qDebug("%lld bytes read from channel %d", bytesread, i);
    }
    output->write((char *)sdata, maxbytesread);

    qDebug("mixer %X looping", (quint64)this);
    timer.start(0);
    return;
    //}
}

void WPMixer::truncateAdd(WPWave::WaveDataType &a, WPWave::WaveDataType b)
{
    qint64 s = (qint64)a + (qint64)b;
    if (s > WPWave::maxWaveDataType)
    {
        a = WPWave::maxWaveDataType;
        return;
    }
    if (s < WPWave::minWaveDataType)
    {
        a = WPWave::minWaveDataType;
        return;
    }
    a = s;
    return;
}
