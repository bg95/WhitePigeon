#include "WPPipe.h"

WPPipe::WPPipe(QObject *parent) :
    QIODevice(parent),
    que(),
    quesize(0),
    readpos(0),
    def(0),
    suf(-1),
    isclosing(false),
    lock(QMutex::NonRecursive),
    filein(0),
    fileout(0),
    closingtimer(0)
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
    lock(QMutex::NonRecursive),
    filein(0),
    fileout(0),
    closingtimer(0)
{
}

WPPipe::~WPPipe()
{
    qDebug("pipe %X deleted", (quint64)this);
}

bool WPPipe::open(OpenMode mode)
{
    mode |= Unbuffered;
    if ((mode & WriteOnly) == 0)
    {
        closeInput();
        return QIODevice::open(mode); //?
    }
    if (mode != (ReadWrite | Unbuffered))
        return false;
    clear();
    isclosing = false;

    //for debug

    char filename[256];
    sprintf(filename, "%llX.in", (quint64)this);
    filein = new QFile(filename);
    qDebug("filein.open %d", filein->open(QIODevice::WriteOnly));
    sprintf(filename, "%llX.out", (quint64)this);
    fileout = new QFile(filename);
    qDebug("fileout.open %d", fileout->open(QIODevice::WriteOnly));

    return QIODevice::open(mode);
}

void WPPipe::close()
{
    if (!isOpen())
        return;
    //lock.lock();

    if (filein)
    {
        if (filein->isOpen())
        {
            filein->close();
            qDebug("pipe %X filein closed", (quint64)this);
            delete filein;
            filein = 0;
        }
        else
            qWarning("pipe %X filein is not open!", (quint64)this);
    }
    if (fileout)
    {
        if (fileout->isOpen())
        {
            fileout->flush();
            qDebug() << fileout->errorString();
            fileout->close();
            qDebug("pipe %X fileout closed", (quint64)this);
            delete fileout;
            fileout = 0;
        }
        else
            qWarning("pipe %X fileout is not open!", (quint64)this);
    }
    QIODevice::close();
    //lock.unlock();
}

qint64 WPPipe::bytesAvailable() const
{
    return quesize + QIODevice::bytesAvailable();
}

qint64 WPPipe::readData(char *data, qint64 maxlen)
{
    lock.lock();
    qDebug("pipe %X read %d", (quint64)this, maxlen);

    char *data0 = data;
    std::deque<QByteArray *>::iterator iter;
    if (maxlen > quesize)
    {
        qWarning("pipe %X reading too much, %d available", (quint64)this, quesize);
        maxlen = quesize;
        //memset(data, 0, quesize);
        //quesize = 0;
        //return quesize;
    }
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
    checkDef();
    maxlen = data - data0;
    if (maxlen)
        fileout->write(data0, maxlen);

    if (isclosing && quesize == 0)
    {
        close();
        if (maxlen == 0)
        {
            lock.unlock();
            return -1;
        }
    }
    lock.unlock();
    return maxlen;
}

qint64 WPPipe::writeData(const char *data, qint64 maxlen)
{
    QByteArray *bytearray = new QByteArray(data, maxlen);

    lock.lock();
    qDebug("pipe %X write %d", (quint64)this, maxlen);
    if (maxlen)
        filein->write(data, maxlen);

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

void WPPipe::closeSoon()
{
    if (closingtimer == 0)
    {
        closingtimer = new QTimer();
        closingtimer->setSingleShot(true);
        connect(closingtimer, SIGNAL(timeout()), this, SLOT(closeSlot()));
        closingtimer->start(10);
        //closeSlot();
    }
}

void WPPipe::closeInput()
{
    lock.lock();
    qDebug("pipe %X close input", (quint64)this);
    isclosing = true;
    lock.unlock();
}

void WPPipe::closeSlot()
{
    qDebug("sender %X", (quint64)sender());
    qDebug("pipe %X closeSlot", (quint64)this);
    close();
}

inline void WPPipe::checkDef() const
{
    if (!isclosing && quesize <= def)
        emit deficientInput();
}
inline void WPPipe::checkSuf() const
{
    if (suf != -1 && quesize > suf)
        emit sufficientInput();
}
