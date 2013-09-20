#ifndef WPPIPE_H
#define WPPIPE_H

#include <QIODevice>
#include <QMutex>
#include <deque>

class WPPipe : public QIODevice
{
    Q_OBJECT
public:
    explicit WPPipe(QObject *parent = 0);
    explicit WPPipe(qint64 _def, qint64 _suf = -1, QObject *parent = 0);
    ~WPPipe();
    bool atEnd() const
    {
        return bytesAvailable() == 0;
    }
    qint64 bytesToWrite() const
    {
        return 0;
    }
    bool canReadLine() const
    {
        return false;
    }
    bool isSequential() const
    {
        return true;
    }
    bool open(OpenMode mode);
    qint64 bytesAvailable() const;
    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 maxlen);
    void clear();
    void setThresholds(qint64 _def = 0, qint64 _suf = -1);
    bool isClosing() const;

signals: //I don't know whether signals can be const
    void deficientInput() const;
    void sufficientInput() const;
    
public slots:
    void closeInput(); //After calling this function, the WPPipe will close when all its buffer is read

private:
    inline void checkDef() const;
    inline void checkSuf() const;
    std::deque<QByteArray *> que;
    qint64 quesize, readpos;
    qint64 def, suf;
    bool isclosing; //correspond to closeInput();
    QMutex lock; //make sure to resopnd to sufficientInput() to prevent writeData(...,...) keeping the lock locked

};

#endif // WPPIPE_H