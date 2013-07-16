#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    glwidget(this),
    glwidgetR(this),
    glwidgetI(this),
    glwidgetSTFT(this)
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
    glwidget.setGeometry(0, 0, width(), height() / 5);
    glwidgetR.setGeometry(0, height() / 5 + 1, width(), height() / 5 - 1);
    glwidgetI.setGeometry(0, height() / 5 * 2 + 1, width(), height() / 5 - 1);
    glwidgetSTFT.setGeometry(0, height() / 5 * 3 + 1, width(), height() / 5 * 2 - 1);
}

void MainWindow::waveDecodeFinished()
{
    wave.Gabor(64, 8);
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
    wave._Gabor(64, 8);
    wave.FFT();
    wave._FFT();

    glwidget.setRange(0, 10000, -32768.0, 32768.0);
    for (int i = 0; i < wave.data.size(); i++)
        glwidget.addPoint(i, wave.data[i], 0.0, 0.0, 1.0);
    glwidget.repaint();

    glwidgetR.setRange(0, wave.FFTdata.size() / 20, -wave.FFTdata.size() * 1000, wave.FFTdata.size() * 1000);
    for (int i = 0; i < wave.FFTdata.size(); i++)
        glwidgetR.addLine(i, 0, i, wave.FFTdata[i].real(), 1.0, 1.0, 0.0);
    glwidgetR.repaint();

    glwidgetI.setRange(0, wave.FFTdata.size() / 20, -wave.FFTdata.size() * 1000, wave.FFTdata.size() * 1000);
    for (int i = 0; i < wave.FFTdata.size(); i++)
        glwidgetI.addLine(i, 0, i, wave.FFTdata[i].imag(), 1.0, 1.0, 0.0);
    glwidgetI.repaint();

    glwidgetSTFT.setRange(0.0, wave.STFTdata.size(), 0.0, wave.STFTdata[0].size() / 4.0);
    printf("points: %d", wave.STFTdata.size() * wave.STFTdata[0].size());
    fflush(stdout);
    for (int i = 0; i < wave.STFTdata.size(); i++)
        for (int j = 0; j < wave.STFTdata[i].size(); j++)
        {
            double t = std::abs(wave.STFTdata[i][j]);
            glwidgetSTFT.addPoint(i, j, t / 100000.0, t / 50000.0, t / 25000.0);
        }
    glwidgetSTFT.repaint();
    wave.play();
}
