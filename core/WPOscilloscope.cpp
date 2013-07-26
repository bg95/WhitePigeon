#include "WPOscilloscope.h"

WPOscilloscope::WPOscilloscope(QWidget *parent) :
    QGLWidget(parent)
{
    timer = new QTimer();
    inputdevice = 0;
    connect(timer, SIGNAL(timeout()), this, SLOT(refresh()));

    QAudioFormat format;
    format.setChannelCount(1);
    format.setCodec("audio/x-raw");
    format.setSampleType(QAudioFormat::SignedInt);
    format.setSampleRate(48000);
    format.setSampleSize(8 * sizeof(WPWave::WaveDataType));
    wave.setFormat(format);
}

WPOscilloscope::~WPOscilloscope()
{
    delete timer;
}

void WPOscilloscope::setInputDevice(QIODevice &_inputdevice)
{
    inputdevice = &_inputdevice;
}

void WPOscilloscope::start(quint32 _period, quint32 _length)
{
    period = _period;
    length = _length;
    timer->start(period);
}

void WPOscilloscope::refresh()
{
    QByteArray bytearray(inputdevice->readAll().rightJustified(length * sizeof(WPWave::WaveDataType), 0, true));
    WPWave::WaveDataType *begin = (WPWave::WaveDataType *)bytearray.constData();
    QVector<WPWave::WaveDataType> data;
    for (int i = 0; i < length; i++)
    {
        data.push_back(*begin);
        begin++;
    }

    wave.setData(data);
    printf("oscilloscope: input length = %d\n", wave.data.size());
    wave.FFT();
    repaint();
}

void WPOscilloscope::initializeGL()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
}

void WPOscilloscope::paintGL()
{
    int i;
    glLoadIdentity();
    glOrtho(0.0, length, -1.0, 1.0, -1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3d(0.0, 1.0, 1.0);
    glBegin(GL_LINE_STRIP);
    for (i = 0; i < wave.data.size(); i++)
        glVertex2d(i, 0.5 + wave.data[i] / 65536.0);
    glEnd();
    glColor3d(1.0, 1.0, 0.0);
    glBegin(GL_LINES);
    for (i = 0; i < wave.FFTdata.size(); i++)
    {
        glVertex2d(double(i) / wave.FFTdata.size() * wave.data.size(), -1.0);
        glVertex2d(double(i) / wave.FFTdata.size() * wave.data.size(), -1.0 + std::abs(wave.FFTdata[i]) / 32768.0 / length * 2.0);
    }
    glEnd();
}

void WPOscilloscope::resizeGL(int w, int h)
{
    if(0 == h)
        h = 1;//防止一条边为0
    glViewport(0, 0, (GLint)w, (GLint)h);//重置当前视口，本身不是重置窗口的，只不过是这里被Qt给封装好了
    glMatrixMode(GL_PROJECTION);//选择投影矩阵
    glLoadIdentity();//重置选择好的投影矩阵
    //gluPerspective(45.0, (GLfloat)width/(GLfloat)height, 0.1, 100.0);//建立透视投影矩阵
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glOrtho(0.0, wave.data.size(), -1.0, 1.0, -1.0, 1.0);
    paintGL();
}
