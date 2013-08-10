#include "WPPipe.h"

WPPipe::WPPipe(QObject *parent) :
    QIODevice(parent),
    que(),
    quesize(0),
    readpos(0),
    def(0),
    suf(-1),
    isclosing(false),
    lock(QMutex::NonRecursive)
{
}

WPPipe::WPPipe(qint64 _def, qint64 _suf, QObject *parent) :
    QIODevice(parent),
    que(),
    quesize(0),
    readpos(0),
    def(_def),
    suf(_suf),
    isclosing(false),
    lock(QMutex::NonRecursive)
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
    isclosing = false;
    return QIODevice::open(mode);
}

qint64 WPPipe::bytesAvailable() const
{
    return quesize + QIODevice::bytesAvailable();
}

qint64 WPPipe::readData(char *data, qint64 maxlen)
{
    lock.lock();

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
        que.pop_front();
        readpos = 0;
    }
    quesize -= data - data0;
    if (isclosing && quesize == 0)
    {
        close();
    }
    checkDef();

    lock.unlock();
    return data - data0;
}

qint64 WPPipe::writeData(const char *data, qint64 maxlen)
{
    QByteArray *bytearray = new QByteArray(data, maxlen);

    lock.lock();

    que.push_back(bytearray);
    quesize += maxlen;
    checkSuf();

    lock.unlock();
    return maxlen;
}

void WPPipe::clear()
{
    lock.lock();
    que.clear();
    quesize = 0;
    readpos = 0;
    checkDef();
    lock.unlock();
}

void WPPipe::setThresholds(qint64 _def, qint64 _suf)
{
    lock.lock();
    def = _def;
    suf = _suf;
    if (isOpen())
    {
        checkDef();
        checkSuf();
    }
    lock.unlock();
}

bool WPPipe::isClosing() const
{
    return isclosing;
}

void WPPipe::closeInput()
{
    lock.lock();
    isclosing = true;
    lock.unlock();
}

inline void WPPipe::checkDef() const
{
    if (!isclosing && quesize <= def)
        deficientInput();
}
inline void WPPipe::checkSuf() const
{
    if (suf != -1 && quesize > suf)
        sufficientInput();
}
