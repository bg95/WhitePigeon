#include "mainwindow.h"
#include <QApplication>
#include <QLabel>
#include <QImage>
#include <QString>
#include <QMessageBox>
#include "Image.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QString  aa;
    aa.setNum(1234);
    QLabel *label=new QLabel(aa);
    label->show();
    MainWindow w;
    w.show();
    QImage image;
    QString str="a.bmp";
    if (image.load("d:/a.bmp") ){
    }


    //image=image.mirrored();


    Image a,b;
    a.Read_Image(image);
    //a.Save_Image(image,0.5);

    //int ff=a.Get_Power();

   // b=a.Fft();




    if (image.save("d:/b.bmp") ) {
    }

    return app.exec();
}
