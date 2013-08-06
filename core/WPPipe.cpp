#include "WPPipe.h"

WPPipe::WPPipe(QObject *parent) :
    QIODevice(parent),
    que(),
    quesize(0),
    readpos(0)
{
}

WPPipe::~WPPipe()
{
}

bool WPPipe::open(OpenMode mode)
{
    if (mode != ReadWrite)
        return false;
    clear();
    return QIODevice::open(mode);
}

qint64 WPPipe::bytesAvailable() const
{
    return quesize;
}

qint64 WPPipe::readData(char *data, qint64 maxlen)
{
    char *data0 = data;
    std::deque<QByteArray *>::iterator iter;
    if (maxlen > quesize)
        maxlen = quesize;
    for (iter = que.begin(); iter != que.end(); iter++)
    {
        if (maxlen < (*iter)->size() - readpos)
        {
            memcpy(data, (*iter)->constData() + readpos, maxlen);
            readpos += maxlen;
            data += maxlen;
            break;
        }
        memcpy(data, (*iter)->constData() + readpos, (*iter)->size() - readpos);
        data += (*iter)->size() - readpos;
        maxlen -= (*iter)->size() - readpos;
        delete (*iter);
        readpos = 0;
    }
    return data - data0;
}

qint64 WPPipe::writeData(const char *data, qint64 maxlen)
{
    QByteArray *bytearray = new QByteArray(data, maxlen);
    que.push_back(bytearray);
    return maxlen;
}

void WPPipe::clear()
{
    que.clear();
    quesize = 0;
    readpos = 0;
}
