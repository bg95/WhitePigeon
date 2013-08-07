#include "Image.h"

#include <QImage>
#include <complex>
#define Pi 3.1415926

//#include <QMessageBox> //调试



Image::Image(){
    Depth = 0;
    LineBytes = 0;
    Width = 0;
    Height = 0;
}

Image::Image(const Image& src){
    Depth = src.Depth;
    LineBytes = src.LineBytes;
    Width = src.Width;
    Height = src.Height;
    Data = new std::complex<double> [Width*Height];
    for (int i = 0; i < Height*Width; i++){
        *(Data+i)=*(src.Data+i);
    }
}

Image& Image::operator =(const Image& src){
    if (Data == src.Data)
       return *this;
    delete[] Data;
    Depth = src.Depth;
    LineBytes = src.LineBytes;
    Width = src.Width;
    Height = src.Height;
    Data = new std::complex<double> [Width*Height];
    for (int i = 0; i < Height*Width; i++){
        *(Data+i)=*(src.Data+i);
    }
    return *this;
}

bool Image::Read_Image(const QImage& srcImage)
{
    const uchar *pImageBytes = srcImage.bits(); //数据首地址
    Depth = srcImage.depth(); //每个像素的bit数
    LineBytes = srcImage.bytesPerLine(); //每行的字节数
    Width = srcImage.width(); //宽
    Height = srcImage.height(); //高
    const uchar *pByte;
    //遍历读取每个像素，并转换为灰度值
    delete[] Data;
    Data = new std::complex<double> [Width*Height];
    for (int i = 0; i < Height; i++){
        for (int j = 0; j < Width; j++){
            if (8 == Depth){ //采用了256色调色板，8位颜色索引
               pByte = pImageBytes + i * LineBytes + j;
               Data[i * Width + j] = std::complex<double> (*pByte, 0);
            }
            else if (32 == Depth){//32位表示，数据格式为0xFFBBGGRR或0xAABBGGRR
                    pByte = pImageBytes + i * LineBytes + j * 4;
                    //根据RGB模式转化成YIQ色彩模式的方式，取Y作为灰度值
                    uchar pixelValue = (uchar)(0.299 * (float)pByte[0] + 0.587 * (float)pByte[1]
                         + 0.114 * (float)pByte[2]);
                    Data[i * Width + j] = std::complex<double> (pixelValue, 0);
                 }
            else
            return 0;
        }
    }
    return 1;
}

bool Image::Save_Image(QImage& destImage,const double& coef){
    LineBytes = destImage.bytesPerLine();
    Depth = destImage.depth();
    int w = destImage.width();
    int h = destImage.height();
    uchar *pImageBytes = destImage.bits();
    uchar *pByte;
    for (int i = 0; i < h; i++){
        for (int j = 0; j < w; j++){
            double spectral = abs(Data[i * Width + j]) * coef; //灰度值调整
            if (spectral > 255)
               spectral=255;
            //根据图像格式写数据
            if (8 == Depth){
               pByte = pImageBytes + i * LineBytes + j;
               *pByte = spectral;
            }
            else if (32 == Depth){
                    pByte = pImageBytes + i * LineBytes + j * 4;
                    pByte[0] = pByte[1] = pByte[2] = spectral;
                 }
                 else
                 return 0;
        }
    }
    return 1;
}

Image::~Image(){
    delete[] Data;
}

Image Image::Fft(const int& Pow) const{

    Image Res(*this);
    if (0 == Pow){
       return Res;
    }
    int n = 1 << Pow;
    Image Src1,Src2;
    Src1.Data=new std::complex<double>[n/2];
    Src2.Data=new std::complex<double>[n/2];
    for (int i = 0; i < n / 2; i++){
        Src1.Data[i] = Data[i*2];
        Src2.Data[i] = Data[i*2+1];
    }
    Image Res1 = Src1.Fft(Pow-1);
    Image Res2 = Src2.Fft(Pow-1);
    double Angle = 2 * Pi / n;
    std::complex<double> wn(cos(Angle),sin(Angle));
    std::complex<double> w(1,0);
    std::complex<double> u;
    for (int i = 0; i < n / 2; i++){ //蝶形算法
        u = w * Res2.Data[i];
        Res.Data[i] = Res1.Data[i] + u;
        Res.Data[i + n / 2] = Res2.Data[i] - u;
        w = w * wn;
    }
    return Res;
}

Image Image::Fft() const{
    return Fft(Get_Power());
}

int Image::Get_Power() const{
    int Power=1,sum=1;
    while (sum<Width*Height){
        sum*=2;
        Power++;
    }
    return Power;
}
