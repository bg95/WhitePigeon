#ifndef WPPIPE_H
#define WPPIPE_H

#include <QIODevice>
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
        return bytesAvailable() == false;
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

signals:
    void deficientInput();
    void sufficientInput();
    
public slots:

private:
    inline void checkDef();
    inline void checkSuf();
    std::deque<QByteArray *> que;
    qint64 quesize, readpos;
    qint64 def, suf;
};

#endif // WPPIPE_H
