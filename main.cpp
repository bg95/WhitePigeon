#include "UI/mainwindow.h"
#include <QApplication>

#include "OscilloscopeWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow window;
    window.show();
    return a.exec();
}
