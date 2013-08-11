#include "WPMixer.h"

WPMixer::WPMixer(QObject *parent) :
    QObject(parent),
    channel(0),
    chcnt(0),
    output(0),
    sdata(0),
    tdata(0)
{
    timer.setSingleShot(true);
    connect(&timer, SIGNAL(timeout()), this, SLOT(sumUp()));
    setReadLength(1);
}

WPMixer::~WPMixer()
{
}

bool WPMixer::openInputChannels(int number_of_channels)
{
    chcnt = number_of_channels;
    channel = new WPPipe[chcnt];
    int i;
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

void WPMixer::setWatingTime(int msec)
{
    waitingtime = msec;
}

void WPMixer::setReadLength(quint64 length)
{
    if (sdata != 0)
        delete[] sdata;
    if (tdata != 0)
        delete[] tdata;
    readlength = length;
    sdata = new WPWave::WaveDataType[readlength];
    tdata = new WPWave::WaveDataType[readlength];
}

void WPMixer::start()
{
    if (chcnt == 0)
    {
        qCritical("Input channels not opened!");
        return;
    }
    timer.start(0);
}

void WPMixer::sumUp()
{
    //assume no other threads are reading the channel
    int i, j;
    quint64 bytesavailable, maxbytesread;
    quint64 readlengthbytes = readlength * sizeof(WPWave::WaveDataType);
    bool existopen;
    while (true)
    {
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
            timer.start(waitingtime);
            return;
        }
        if (!existopen)
        {
            delete[] channel;
            chcnt = 0;
            allInputClosed();
            return;
        }
        memset(sdata, 0, readlengthbytes);
        maxbytesread = 0;
        for (i = 0; i < chcnt; i++)
        {
            if (channel[i].isOpen())
            {
                bytesavailable = channel[i].bytesAvailable();
                if (bytesavailable >= readlengthbytes)
                {
                    if (maxbytesread < readlengthbytes)
                        maxbytesread = readlengthbytes;
                    channel[i].read((char *)tdata, readlengthbytes);
                    for (j = 0; j < readlength; j++)
                        truncateAdd(sdata[j], tdata[j]);
                }
                else
                {
                    if (maxbytesread < bytesavailable)
                        maxbytesread = bytesavailable;
                    qDebug("channel %d is closing and has only %d bytes available", i, bytesavailable);
                    channel[i].read((char *)tdata, bytesavailable);
                    for (j = 0; j < bytesavailable / sizeof(WPWave::WaveDataType); j++)
                        truncateAdd(sdata[j], tdata[j]);
                }
            }
        }
        output->write((char *)sdata, maxbytesread);
    }
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
