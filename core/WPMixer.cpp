#include "WPMixer.h"

WPMixer::WPMixer(QObject *parent) :
    QObject(parent),
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

void WPMixer::setWatingTime(int msec)
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

void WPMixer::start()
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
    timer.start(0);
}

void WPMixer::sumUp()
{
    //assume no other threads are reading the channel
    qint32 i, j;
    qint64 bytesavailable, maxbytesread, bytesread;
    qint64 readlengthbytes = readlength * sizeof(WPWave::WaveDataType);
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
            qDebug("mixer waiting");
            timer.start(waitingtime);
            return;
        }
        if (!existopen)
        {
            delete[] channel;
            chcnt = 0;
            /*//debug
            if (filein)
            {
                if (filein->isOpen())
                {
                    filein->close();
                    qDebug("mixer %X filein closed", (quint64)this);
                    delete filein;
                    filein = 0;
                }
                else
                    qWarning("mixer %X filein is not open!", (quint64)this);
            }
            if (fileout)
            {
                if (fileout->isOpen())
                {
                    fileout->flush();
                    //qDebug() << fileout->errorString();
                    fileout->close();
                    qDebug("mixer %X fileout closed", (quint64)this);
                    delete fileout;
                    fileout = 0;
                }
                else
                    qWarning("mixer %X fileout is not open!", (quint64)this);
            }*/
            //debug end
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
                    bytesread = channel[i].read((char *)tdata, readlengthbytes);
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
            //if (bytesread > 0)
            //    filein->write((char *)tdata, bytesread);
            qDebug("%lld bytes read from channel %d", bytesread, i);
        }/*
        for (i = 0; i < maxbytesread / sizeof(WPWave::WaveDataType); i++)
            if (sdata[i] != tdata[i])
            {
                qDebug("not equal at %d", i);
            }*/
        //fileout->write((char *)sdata, maxbytesread);
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
