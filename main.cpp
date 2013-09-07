#include "mainwindow.h"
#include <QApplication>

#include "OscilloscopeWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow window;
    window.showMaximized();

    return a.exec();
}
