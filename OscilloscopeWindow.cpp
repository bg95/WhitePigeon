#include "OscilloscopeWindow.h"
//#include "ui_mainwindow.h"
#include "WPScore/WPNote.h"
#include "core/WPSynthesizer.h"
//#include "core/WPTuningFork.h"
#include "core/WPPipe.h"

OscilloscopeWindow::OscilloscopeWindow(QWidget *parent) :
    QMainWindow(parent),/*
    ui(new Ui::OscilloscopeWindow),
    glwidget(this),
    glwidgetR(this),
    glwidgetI(this),
    glwidgetSTFT(this),*/
    oscilloscope(new WPOscilloscope(this)),
    audioinput(0)
{
    //ui->setupUi(this);
    setGeometry(100, 50, 1000, 600);
    //pipe = new WPPipe();
}

OscilloscopeWindow::~OscilloscopeWindow()
{
    //delete ui;
    delete oscilloscope;
    if (audioinput)
        delete audioinput;
    //delete pipe;
}

void OscilloscopeWindow::showEvent(QShowEvent *)
{
    //WPCallbackManager::init();
    //wave.readFile("/home/pt-cr/Projects/build-WhitePigeon-Desktop-Debug/wave suprised.wav");
    //connect(&wave, SIGNAL(finished()), this, SLOT(waveDecodeFinished()));
    qDebug("OscilloscopeWindow %llX in thread %llX\n", (quint64)this, (quint64)QThread::currentThread());

    //WPNote note1(0, Fraction(1, 16)), note2(4, Fraction(1, 16)), note3(7, Fraction(1, 16));
    //WPNote note4(-5, Fraction(1, 4)), note5(-1, Fraction(1, 4)), note6(2, Fraction(1, 4));
    WPNote longnote(0, Fraction(10, 1));
    /*
    WPNote note1(0, Fraction(1, 1)), note2(2, Fraction(1, 1)), note3(4, Fraction(1, 1)), note4(5, Fraction(1, 1)), note5(7, Fraction(1, 1)), note6(9, Fraction(1, 1));
    WPNote note11(0, Fraction(2, 1)), note22(2, Fraction(2, 1)), note33(4, Fraction(2, 1)), note44(5, Fraction(2, 1)), note55(7, Fraction(2, 1));
    WPNote notel1(-12 + 12, Fraction(1, 2)), notel5(-17 + 12, Fraction(1, 2)), notel3(-20 + 12, Fraction(1, 2));
    WPNote notet1(0, Fraction(1, 3)), notet2(2, Fraction(1, 3)), notet3(4, Fraction(1, 3)), notet4(5, Fraction(1, 3)), notet5(7, Fraction(1, 3)), notet6(9, Fraction(1, 3));
*/
    WPNote note1(0-9, Fraction(1, 4)), note2(2-9, Fraction(1, 4)), note3(4-9, Fraction(1, 4)), note4(5-9, Fraction(1, 4)), note5(7-9, Fraction(1, 4)), note6(9-9, Fraction(1, 4));
    WPNote note11(0-9, Fraction(2, 4)), note22(2-9, Fraction(2, 4)), note33(4-9, Fraction(2, 4)), note44(5-9, Fraction(2, 4)), note55(7-9, Fraction(2, 4));
    WPNote notel1(-12 -9, Fraction(1, 8)), notel5(-17 -9, Fraction(1, 8)), notel3(-20 -9, Fraction(1, 8));
    WPNote notel77(-1-9, Fraction(2, 4));
    /**/WPNote notet1(0-9, Fraction(1, 3)), notet2(2-9, Fraction(1, 3)), notet3(4-9, Fraction(1, 3)), notet4(5-9, Fraction(1, 3)), notet5(7-9, Fraction(1, 3)), notet6(9-9, Fraction(1, 3));
    Fraction pos;
    score = new WPScore;
/*
    score->lockForWrite();
    score->newPart("whitepig");
    score->unlock();


    score->lockForRead();
    score->getPartList()[0].insertMultinote(WPPosition(Fraction(0, 1)), WPMultinote(longnote));
    score->getPartList()[0].insertMultinote(WPPosition(Fraction(10, 1)), WPMultinote(longnote));
    score->getPartList()[0].startFrom(WPPosition(Fraction(0, 1)));
    score->unlock();
*/

    score->lockForWrite();
    score->newPart("whitepig");
    //score->newPart("white");
    //score->newPart("swine");
    score->unlock();
	score->lockForWrite();
    pos = Fraction(0, 1);
    for (int i = 0; i < 10; i++)
    {
        score->getPartList()[0].insertMultinote(WPPosition(pos), WPMultinote(note1));
        pos += note1.getLength();
        score->getPartList()[0].insertMultinote(WPPosition(pos), WPMultinote(note2));
        pos += note2.getLength();
        score->getPartList()[0].insertMultinote(WPPosition(pos), WPMultinote(note3));
        pos += note3.getLength();
    }
    score->getPartList()[0].insertProperty(WPProperty(WPInterval(WPPosition(Fraction(9, 4)), WPPosition(Fraction(48, 1))), "WPTempoModifier 140.0"));
    score->getPartList()[0].insertProperty(WPProperty(WPInterval(WPPosition(Fraction(0, 1)), WPPosition(Fraction(3, 4))), "WPSlur"));
    score->getPartList()[0].insertProperty(WPProperty(WPInterval(WPPosition(Fraction(3, 4)), WPPosition(Fraction(9, 4))), "WPAmpModifier 0.5"));
    score->getPartList()[0].startFrom(WPPosition(Fraction(0, 1)));
	score->unlock();

/*
    score->lockForWrite();
    for (int i = 0; i < 16 * 3; i++)
    {
        score->getPartList()[1].insertMultinote(WPPosition(Fraction(2 * i, 8)), WPMultinote((i % 8 != 7) ? notel1 : notel3));
        score->getPartList()[1].insertMultinote(WPPosition(Fraction(2 * i + 1, 8)), WPMultinote(notel5));
		score->getPartList()[1].insertProperty(WPProperty(WPInterval(WPPosition(Fraction(2 * i, 8)), WPPosition(Fraction(2 * i + 2, 8))), "WPSlur "));
    }
    score->getPartList()[1].startFrom(WPPosition(Fraction(0, 1)));
    //score->getPartList()[1].setToBeMuted();
	score->unlock();
*/
/*
    score->lockForWrite();
    score->load("pi2.wps");
	score->unlock();*/

    file = new QFile("wave.out");
    file->open(QIODevice::WriteOnly);
    qDebug("part num = %d\n", (int)score->getPartList().size());
    controller = new WPSynthesisController;
    //connect(controller, SIGNAL(synthesisFinished()), this, SLOT(waveDecodeFinished()));
    //controller->synthesizeAndOutput(*score, file);
    qDebug("part volume = %lf", score->getPartList()[0].getVolume());

    //set plugin directories
    /*
    WPPropertyAndModifiers::ModifierPluginDir = "/home/pt-cr/Projects/WhitePigeon/WhitePigeon/plugins/";
    WPDLLTimbreManager::TimbrePluginDir = "/home/pt-cr/Projects/WhitePigeon/WhitePigeon/plugins/";
    */

    controller->synthesizeAndPlay(*score); /// prevent you from playing music!
    qDebug("part volume = %lf", score->getPartList()[0].getVolume());

    audioinput = new QAudioInput(WPWave::defaultAudioFormat());
    audioinput->setVolume(0.1);
    oscilloscope->setInputDevice(*audioinput->start());
    //oscilloscope->setInputDevice(*pipe);
    //oscilloscope->setInputDevice(*controller->synthesize(*score));
    oscilloscope->start(100, 4096);
    //oscilloscope->start(100, 512);
}

void OscilloscopeWindow::resizeEvent(QResizeEvent *)
{/*
    glwidget.setGeometry(0, 0, width(), height() / 5);
    glwidgetR.setGeometry(0, height() / 5 + 1, width(), height() / 5 - 1);
    glwidgetI.setGeometry(0, height() / 5 * 2 + 1, width(), height() / 5 - 1);
    glwidgetSTFT.setGeometry(0, height() / 5 * 3 + 1, width(), height() / 5 * 2 - 1);*/
    oscilloscope->setGeometry(0, 0, width(), height());
}

void OscilloscopeWindow::hideEvent(QHideEvent *)
{
    //wave.stop();
    delete controller;
    delete score;
    //controller.stopAll();
}

void OscilloscopeWindow::waveDecodeFinished()
{/*
    qDebug("synthesis finished");
    file->close();
    file->open(QIODevice::ReadOnly);
    QAudioOutput *audiooutput = new QAudioOutput(WPWave::defaultAudioFormat());
    audiooutput->start(file);*/
    //qDebug("decode finished");
    /*
    qWarning("Gabor");
    wave.Gabor(256, 512);
    QVector<QVector<std::complex<double> > > tmp;
    tmp.clear();
    for (int i = 0; i < wave.STFTdata.size(); i++)
    {
        tmp.push_back(QVector<std::complex<double> >());
        tmp[i].fill(0.0, wave.STFTdata[i].size());
        for (int j = 0; j < wave.STFTdata[i].size(); j++)
            tmp[i][(j * 2) % wave.STFTdata[i].size()] += wave.STFTdata[i][j];
    }
    //wave.STFTdata = tmp;
    wave._Gabor(256, 512);
    qWarning("FFT");
    wave.FFT();
    wave._FFT();
*//*
    qWarning("drawing");
    glwidget.setRange(0, 10000, -32768.0, 32768.0);
    for (int i = 0; i < wave.data.size(); i++)
        glwidget.addPoint(i, wave.data[i], 0.0, 0.0, 1.0);
    glwidget.repaint();
*//*
    glwidgetR.setRange(0, wave.FFTdata.size() / 20, -wave.FFTdata.size() * 1000, wave.FFTdata.size() * 1000);
    for (int i = 0; i < wave.FFTdata.size(); i++)
        glwidgetR.addLine(i, 0, i, wave.FFTdata[i].real(), 1.0, 1.0, 0.0);
    glwidgetR.repaint();

    glwidgetI.setRange(0, wave.FFTdata.size() / 20, -wave.FFTdata.size() * 1000, wave.FFTdata.size() * 1000);
    for (int i = 0; i < wave.FFTdata.size(); i++)
        glwidgetI.addLine(i, 0, i, wave.FFTdata[i].imag(), 1.0, 1.0, 0.0);
    glwidgetI.repaint();

    glwidgetSTFT.setRange(0.0, wave.STFTdata.size(), 0.0, wave.STFTdata[0].size() / 4.0);
    fflush(stdout);
    for (int i = 0; i < wave.STFTdata.size(); i++)
        for (int j = 0; j < wave.STFTdata[i].size(); j++)
        {
            double t = std::abs(wave.STFTdata[i][j]);
            if (t >= 10.0)
                glwidgetSTFT.addPoint(i, j, t / 16000.0, t / 4000.0, t / 1000.0);
        }
    printf("points: %d", glwidgetSTFT.getNumPoints());
    glwidgetSTFT.repaint();
    wave.play();*/
}
