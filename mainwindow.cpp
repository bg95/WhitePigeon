#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    glwidget(this),
    glwidget2(this)
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
    glwidget.setGeometry(0, 0, width(), height() / 2);
    glwidget2.setGeometry(0, height() / 2 + 1, width(), height() / 2);
}

void MainWindow::waveDecodeFinished()
{
    wave.FFT();
    wave._FFT();
    wave.play();

    glwidget.setRange(0, 10000, -32768.0, 32768.0);
    for (int i = 0; i < wave.data.size(); i++)
        glwidget.addPoint(i, wave.data[i]);
    glwidget.repaint();

    glwidget2.setRange(0, wave.FFTdata.size(), 0, wave.FFTdata.size() * 1000);
    for (int i = 0; i < wave.FFTdata.size(); i++)
        glwidget2.addLine(i, 0, i, wave.FFTdata[i].real());
    glwidget2.repaint();
}
