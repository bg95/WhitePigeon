#ifndef IMAGE_H
#define IMAGE_H

#include <QImage>
#include <complex>
#include <deque>
//#include <QMessageBox> //调试
#include<QDebug>

class Image
{
public:
    Image();//OK

    Image(const Image&);//OK

    Image& operator =(const Image&);//OK

    virtual ~Image();//OK

    bool Read_Image(const QImage&);//OK

    bool Save_Image(QImage&,const double&);//OK

    void Fourier();

    void iFourier();

    int Get_Width()const;

    int Get_Height()const;

    double Get_Data(const int&, const int&)const;

    void Denoise();

    void Write_Data(const int&, const int&, const int);//仅仅用来调试！！！！！！！！！！！！！！！！


private:
    void AdjustImageSize();//??将长宽改为2的幂次

    Image Fft(const int&);//??

    Image iFft(const int&);//??


    void Denoise_0();

    void Denoise_1();

    void Denoise_2();

    void Denoise_3();

private:
    std::complex<double>* Data; //数据
    int Width; //宽
    int Height; //高

	static const double Pi=3.1415926;
	static const double Const_Color=2;

};

#endif // IMAGE_H
//    int Get_Power()const;
