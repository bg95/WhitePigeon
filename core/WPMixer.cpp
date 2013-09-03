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
    timer.start(0);
}

void WPMixer::sumUp()
{
    //assume no other threads are reading the channel
    int i, j;
    bool existopen;
    while (true)
    {
        existopen = false;
        for (i = 0; i < chcnt; i++)
        {
            if (channel[i].isOpen())
                existopen = true;
            if (channel[i].isOpen() && !channel[i].isClosing() &&
                channel[i].bytesAvailable() < readlength * sizeof(WPWave::WaveDataType))
                break;
        }
        if (i < chcnt)
        {
            timer.start(waitingtime);
            return;
        }
        if (!existopen)
        {
            allInputClosed();
            return;
        }
        memset(sdata, 0, sizeof(WPWave::WaveDataType) * readlength);
        for (i = 0; i < chcnt; i++)
        {
            if (channel[i].isOpen())
            {
                if (channel[i].bytesAvailable() >= readlength * sizeof(WPWave::WaveDataType))
                {
                    channel[i].read((char *)tdata, readlength * sizeof(WPWave::WaveDataType));
                    for (j = 0; j < readlength; j++)
                        truncateAdd(sdata[j], tdata[j]);
                }
                else
                {
                    channel[i].read((char *)tdata, channel[i].bytesAvailable());
                    for (j = 0; j < channel[i].bytesAvailable() / sizeof(WPWave::WaveDataType); j++)
                        truncateAdd(sdata[j], tdata[j]);
                }
            }
        }
        output->write((char *)sdata, readlength * sizeof(WPWave::WaveDataType));
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
