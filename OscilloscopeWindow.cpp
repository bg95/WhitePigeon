#include "OscilloscopeWindow.h"
//#include "ui_mainwindow.h"
#include "WPScore/WPNote.h"
#include "core/WPSynthesizer.h"
#include "core/WPTuningFork.h"
#include "core/WPPipe.h"

OscilloscopeWindow::OscilloscopeWindow(QWidget *parent) :
    QMainWindow(parent),
    /*ui(new Ui::OscilloscopeWindow),
    glwidget(this),
    glwidgetR(this),
    glwidgetI(this),
    glwidgetSTFT(this),*/
    oscilloscope(this)
{
    //ui->setupUi(this);
    setGeometry(100, 50, 1000, 600);
    pipe = new WPPipe();
}

OscilloscopeWindow::~OscilloscopeWindow()
{
    //delete ui;
    delete audioinput;
    delete pipe;
}

void OscilloscopeWindow::showEvent(QShowEvent *)
{
    //wave.readFile("/home/pt-cr/Projects/build-WhitePigeon-Desktop-Debug/wave suprised.wav");
    //connect(&wave, SIGNAL(finished()), this, SLOT(waveDecodeFinished()));

	WPNote note1(0, Fraction(1, 1)), note2(4, Fraction(1, 16)), note3(7, Fraction(1, 16));
	WPNote note4(-5, Fraction(1, 1)), note5(-1, Fraction(1, 16)), note6(2, Fraction(1, 16));
    WPNote longnote(0, Fraction(10, 1));
/*
    WPWave *twave = synthesizer.synthesize(note1);
    //twave->play();
    wave.clear();
    wave.setFormat(WPWave::defaultAudioFormat());
    wave.mixWith(0.5, *twave, 0.5);
    delete twave;
    qDebug("synthesis finished");
    wave.play();
*/
	score = new WPScore;
	score->lockForWrite();
	score->newPart("whitepig");
	score->unlock();
	score->lockForRead();
	qDebug("part num = %d\n", score->getPartList().size());
	score->getPartList()[0].insertMultinote(WPPosition(Fraction(0, 1)), WPMultinote(note1));
	score->getPartList()[0].insertMultinote(WPPosition(Fraction(1, 1)), WPMultinote(note2));
	score->getPartList()[0].insertMultinote(WPPosition(Fraction(2, 1)), WPMultinote(note3));
	score->getPartList()[0].startFrom(WPPosition(Fraction(0, 1)));
	score->unlock();
	score->lockForWrite();
	score->newPart("white");
	score->unlock();
	score->lockForRead();
    score->getPartList()[1].insertMultinote(WPPosition(Fraction(0, 1)), WPMultinote(note4));
    score->getPartList()[1].insertMultinote(WPPosition(Fraction(200, 1)), WPMultinote(note5));
    score->getPartList()[1].insertMultinote(WPPosition(Fraction(500, 2)), WPMultinote(note6));
    score->getPartList()[1].startFrom(WPPosition(Fraction(0, 1)));
	score->unlock();

	score->lockForWrite();
	score->save("pig.wps");
	score->close();
	score->load("pig.wps");
	score->unlock();

    file = new QFile("wave.out");
    file->open(QIODevice::WriteOnly);
    controller = new WPSynthesisController;
    //connect(controller, SIGNAL(synthesisFinished()), this, SLOT(waveDecodeFinished()));
    //controller->synthesizeAndOutput(*score, file);
    controller->synthesizeAndPlay(*score);
/*
    WPSynthesizer synthesizer;
    WPTuningFork tuningfork;
    synthesizer.loadTimbre(&tuningfork);
    qDebug("synthesizer constructed");
    pipe->open(QIODevice::ReadWrite);

    synthesizer.setOutputDevice(*pipe);
    synthesizer.startSynthesis(score->getPartList()[0]);
    static QAudioOutput *audiooutput = new QAudioOutput(WPWave::defaultAudioFormat());
    audiooutput->start(pipe);
*/

    audioinput = new QAudioInput(WPWave::defaultAudioFormat());
    audioinput->setVolume(0.1);
    oscilloscope.setInputDevice(*audioinput->start());
    //oscilloscope.setInputDevice(*pipe);
    //oscilloscope.setInputDevice(*controller->synthesize(*score));
    //oscilloscope.start(100, 4096);
    oscilloscope.start(100, 512);
}

void OscilloscopeWindow::resizeEvent(QResizeEvent *)
{/*
    glwidget.setGeometry(0, 0, width(), height() / 5);
    glwidgetR.setGeometry(0, height() / 5 + 1, width(), height() / 5 - 1);
    glwidgetI.setGeometry(0, height() / 5 * 2 + 1, width(), height() / 5 - 1);
    glwidgetSTFT.setGeometry(0, height() / 5 * 3 + 1, width(), height() / 5 * 2 - 1);*/
    oscilloscope.setGeometry(0, 0, width(), height());
}

void OscilloscopeWindow::hideEvent(QHideEvent *)
{
    //wave.stop();
    delete score;
}

void OscilloscopeWindow::waveDecodeFinished()
{
    qDebug("synthesis finished");
    file->close();
    file->open(QIODevice::ReadOnly);
    QAudioOutput *audiooutput = new QAudioOutput(WPWave::defaultAudioFormat());
    audiooutput->start(file);
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
