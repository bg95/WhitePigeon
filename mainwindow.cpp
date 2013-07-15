#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    glwidget(this),
    glwidgetR(this),
    glwidgetI(this)
{
    //ui->setupUi(this);
    setGeometry(100, 100, 1000, 600);
    wave.readFile("/home/pt-cr/Projects/build-WhitePigeon-Desktop-Debug/wave.wav");
    connect(&wave, SIGNAL(finished()), this, SLOT(waveDecodeFinished()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    glwidget.setGeometry(0, 0, width(), height() / 3);
    glwidgetR.setGeometry(0, height() / 3 + 1, width(), height() / 3 - 1);
    glwidgetI.setGeometry(0, height() / 3 * 2 + 1, width(), height() / 3 - 1);
}

void MainWindow::waveDecodeFinished()
{
    //wave.Gabor(1, 1);
    //wave._Gabor(1, 1);
    wave.FFT();
    wave._FFT();
    wave.play();

    glwidget.setRange(0, 10000, -32768.0, 32768.0);
    for (int i = 0; i < wave.data.size(); i++)
        glwidget.addPoint(i, wave.data[i]);
    glwidget.repaint();

    glwidgetR.setRange(0, wave.FFTdata.size() / 20, -wave.FFTdata.size() * 1000, wave.FFTdata.size() * 1000);
    for (int i = 0; i < wave.FFTdata.size(); i++)
        glwidgetR.addLine(i, 0, i, wave.FFTdata[i].real());
    glwidgetR.repaint();

    glwidgetI.setRange(0, wave.FFTdata.size() / 20, -wave.FFTdata.size() * 1000, wave.FFTdata.size() * 1000);
    for (int i = 0; i < wave.FFTdata.size(); i++)
        glwidgetI.addLine(i, 0, i, wave.FFTdata[i].imag());
    glwidgetI.repaint();
}
