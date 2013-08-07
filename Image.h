#ifndef IMAGE_H
#define IMAGE_H

#include <QImage>
#include <complex>

class Image
{
public:
    Image();

    Image(const Image&);

    bool Read_Image(const QImage&);

    bool Save_Image(QImage&,const double&);

    Image Fft()const;

    virtual ~Image();

    Image& operator =(const Image&);

    int Get_Power()const;
    Image Fft(const int&)const;
private:
    std::complex<double>* Data; //数据





private:
    int Depth; //每个像素的bit数
    int LineBytes; //每行的字节数
    int Width; //宽
    int Height; //高

};

#endif // IMAGE_H
