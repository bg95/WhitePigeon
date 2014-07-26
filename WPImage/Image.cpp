#include "Image.h"

const double Image::Pi=3.1415926;
const double Image::Const_Color=2;

Image::Image()
{
    Width = 0;
    Height = 0;
    Data=NULL;
}

Image::Image(const Image& src)
{
    Width = src.Width;
    Height = src.Height;
    Data = new std::complex<double> [Width*Height];
    for (int i = 0; i < Height*Width; i++)
    {
        *(Data+i)=*(src.Data+i);
    }
}

Image& Image::operator   =(const Image& src)
{
    if (Data == src.Data)
       return *this;
    if (Data)
       delete[] Data;
    Width = src.Width;
    Height = src.Height;
    Data = new std::complex<double> [Width*Height];
    for (int i = 0; i < Height*Width; i++)
    {
        *(Data+i)=*(src.Data+i);
    }
    return *this;
}

Image::~Image()
{
    if (Data)
       delete[] Data;
}

bool Image::Read_Image(const QImage& srcImage)
{
    const uchar *pImageBytes = srcImage.bits(); //数据首地址
    const int Depth = srcImage.depth(); //每个像素的bit数
    const int LineBytes = srcImage.bytesPerLine(); //每行的字节数
    Width = srcImage.width(); //宽
    Height = srcImage.height(); //高
    const uchar *pByte;
    //遍历读取每个像素，并转换为灰度值
    if (Data)
       delete[] Data;
    Data = new std::complex<double> [Width*Height];
    for (int i = 0; i < Height; i++)
    {
        for (int j = 0; j < Width; j++)
        {
            if (8 == Depth){ //采用了256色调色板，8位颜色索引
               pByte = pImageBytes + i * LineBytes + j;
               Data[i * Width + j] = std::complex<double> (*pByte, 0);
            }
            else if (32 == Depth) //32位表示，数据格式为0xFFBBGGRR或0xAABBGGRR
                 {
                    pByte = pImageBytes + i * LineBytes + j * 4;
                    //根据RGB模式转化成YIQ色彩模式的方式，取Y作为灰度值
                    uchar pixelValue = (uchar)(0.33 * (float)pByte[0] + 0.33 * (float)pByte[1]
                         + 0.33 * (float)pByte[2]);
                    Data[i * Width + j] = std::complex<double> (pixelValue, 0);
                 }
            else
            return 0;
        }
    }
    return 1;
}

bool Image::Save_Image(QImage& destImage, const double& coef)
{
    const int LineBytes = destImage.bytesPerLine();
    const int Depth = destImage.depth();
    int w = destImage.width();
    int h = destImage.height();
    uchar *pImageBytes = destImage.bits();
    uchar *pByte;

    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            double spectral;
            if (i<Height && j<Width)
               spectral= abs(Data[i * Width + j]) * coef; //灰度值调整
            if (spectral > 255)
               spectral=255;
            //根据图像格式写数据
            if (8 == Depth)
            {
               pByte = pImageBytes + i * LineBytes + j;
               *pByte = round(spectral);
            }
            else if (32 == Depth)
                 {
                    pByte = pImageBytes + i * LineBytes + j * 4;
                    pByte[0] = pByte[1] = pByte[2] = spectral;
                 }
                 else
                 return 0;
        }
    }
    return 1;
}

void Image::Fourier()
{
    (*this).AdjustImageSize();
    Image Temp1, Temp2;
    int Size1 = 1, Size2 = 1;

    while ((1<<Size1) != Width)
          Size1++;
    Temp1.Height = 1;
    Temp1.Width  = Width;
    Temp1.Data=new std::complex<double> [Width];
 //qDebug()<< Width;
    for (int i = 0; i < Height; i++)
    {
        for (int j = 0; j < Width; j++)
        {
            Temp1.Data[j]=Data[i * Width + j];
        //    if (i==0)
       //         qDebug()<< Temp1.Data[j].real()<<" "<<Temp1.Data[j].imag();
        }//qDebug()<<1000000000;
        Temp1=Temp1.Fft(Size1);
        for (int j = 0; j < Width; j++)
        {
            Data[i * Width + j]=Temp1.Data[j];
          //  if (i==0)
                // qDebug()<< Temp1.Data[j].real()<<" "<<Temp1.Data[j].imag();
        }
// qDebug() << i;
    }
//return;
    Temp2.Data=new std::complex<double> [Height];
    while ((1<<Size2) != Height)
          Size2++;
    Temp2.Height = 1;
    Temp2.Width  = Height;
    for (int i = 0; i < Width; i++)
    {
        for (int j = 0; j < Height; j++)
        {
            Temp2.Data[j]=Data[j * Width + i];
        }
        Temp2=Temp2.Fft(Size2);
        for (int j = 0; j < Height; j++)
        {
            Data[j * Width + i]=Temp2.Data[j];
        }
    }
}

void Image::iFourier()
{
    Image Temp1, Temp2;
    int Size1 = 1, Size2 = 1;

    Temp2.Data=new std::complex<double> [Height];


    while ((1<<Size1) != Width)
          Size1++;
    Temp1.Height = 1;
    Temp1.Width  = Width;
    Temp1.Data=new std::complex<double> [Width];
 //qDebug()<< Width;
    for (int i = 0; i < Height; i++)
    {
        for (int j = 0; j < Width; j++)
        {
            Temp1.Data[j]=Data[i * Width + j];
        //    if (i==0)
       //         qDebug()<< Temp1.Data[j].real()<<" "<<Temp1.Data[j].imag();
        }//qDebug()<<1000000000;
        Temp1=Temp1.iFft(Size1);
        for (int j = 0; j < Width; j++)
        {
            Data[i * Width + j]=Temp1.Data[j];
          //  if (i==0)
                // qDebug()<< Temp1.Data[j].real()<<" "<<Temp1.Data[j].imag();
        }
// qDebug() << i;
    }
//return;
    while ((1<<Size2) != Height)
          Size2++;
    Temp2.Height = 1;
    Temp2.Width  = Height;
    for (int i = 0; i < Width; i++)
    {
        for (int j = 0; j < Height; j++)
        {
            Temp2.Data[j]=Data[j * Width + i];
        }
        Temp2=Temp2.iFft(Size2);
        for (int j = 0; j < Height; j++)
        {
            Data[j * Width + i]=Temp2.Data[j];
        }
    }
    //for (int i = 0; i <5; i++)
    //        qDebug()<<int(Data[i].real()*10000)<<" "<<int(Data[i].imag()*10000);
}

int Image::Get_Width()const
{
    return Width;
}

int Image::Get_Height()const
{
    return Height;
}

double Image::Get_Data(const int &i, const int &j)const
{
    if ( i<0 || j<0 || i>=Height || j>= Width)return 0;
     //   qDebug()<<abs(Data[i * Width + j]);
    return abs(Data[i * Width + j]);
}

void Image::Denoise()
{
    Denoise_0();
    Denoise_0();
    Denoise_1();
    //a.Denoise_2();
    //a.Denoise_2();
    Denoise_3();
    //a.Denoise_3();
}

void Image::Write_Data(const int &i, const int &j,const int color)
{
    Data[i * Width + j]=color;
}

//Private:

void Image::AdjustImageSize()
{
    Image res;
    res.Width = 1;
    while (res.Width < Width)
          res.Width *= 2;
    res.Height = 1;
    while (res.Height < Height)
          res.Height *= 2;
    res.Data = new std::complex<double> [res.Width*res.Height];

    for (int i = 0; i < res.Height; i++)
    {
        for (int j = 0; j < res.Width; j++)
        {
            if (i < Height && j < Width)
               res.Data[i * res.Width + j] = Data[i * Width + j];
               else res.Data[i * res.Width + j] = 0;
        }
    }
    (*this)=res;
}

Image Image::Fft(const int& Pow)
{
    Image Res;
    int n = (1 << Pow);
    Res.Data=new std::complex<double>[n];
    Res.Width=n;
    Res.Height=1;
    for (int i = 0; i < n ; i++)
        Res.Data[i] = Data[i];
    if (0 == Pow)
    {
       return Res;
    }
    Image Src1,Src2;
    Src1.Data = new std::complex<double>[n/2];
    Src2.Data = new std::complex<double>[n/2];
    Src1.Width = n/2;
    Src1.Height = 1;
    Src2.Width = n/2;
    Src2.Height = 1;
    for (int i = 0; i < n / 2; i++)
    {
        Src1.Data[i] = Data[i * 2];
        Src2.Data[i] = Data[i * 2 + 1];
    }
    Image Res1;
    Res1 = Src1.Fft(Pow-1);
    Image Res2;
    Res2 = Src2.Fft(Pow-1);

    double Angle = 2.0 * Pi / n;
    std::complex<double> wn = std::polar(1.0, Angle);
    std::complex<double> w(1,0);
    std::complex<double> u;
    for (int i = 0; i < n / 2; i++)
    { //蝶形算法
        u = w * Res2.Data[i];
        Res.Data[i] = Res1.Data[i] + u;
        Res.Data[i + n / 2] = Res1.Data[i] - u;
        w = w * wn;
    }
  //  for (int i = 0; i < n ; i++){
  //      qDebug()<< Pow<<" "<<Res.Data[i].real()<<" "<<Res.Data[i].imag();
  //  }
    return Res;
}

Image Image::iFft(const int& Pow)
{
    Image Temp=(*this);
    int n=1<<Pow;
    for (int i = 0; i < n ; i++)
    {
        Temp.Data[i]=std::complex<double> (Temp.Data[i].real(),-Temp.Data[i].imag());
    }
    Image Res;
    Res=Temp.Fft(Pow);
    for (int i = 0; i < n ; i++)
    {
        Res.Data[i]=std::complex<double> (Res.Data[i].real()/n,-Res.Data[i].imag()/n);
    }
  //  for (int i = 0; i < n ; i++){
  //      qDebug()<< Pow<<" "<<Res.Data[i].real()<<" "<<Res.Data[i].imag();
  //  }
    return Res;
}

void Image::Denoise_0()
{
    /*
    Image Temp=(*this);
    for (int i = 1; i < Height - 1; i++)
        for (int j = 1; j < Width -1; j++){
            //double a=abs (Temp.Data[(i-1) * Width + j-1 ]);
           // if (abs(a-0)>0.1 && abs(a-255)>0.1)
           //     qDebug()<<a;
            double sum = 0;
           // sum += abs (Temp.Data[(i-1) * Width + j-1 ]);
            sum -= abs (Temp.Data[(i-1) * Width + j ]);
          //  sum += abs (Temp.Data[(i-1) * Width + j+1 ]);
            sum -= abs (Temp.Data[i * Width + j-1 ]);
            sum += 5*abs (Temp.Data[i * Width + j ]);
            sum -= abs (Temp.Data[i * Width + j+1 ]);
          //  sum += abs (Temp.Data[(i+1) * Width + j-1 ]);
            sum -= abs (Temp.Data[(i+1) * Width + j ]);
           // sum += abs (Temp.Data[(i+1) * Width + j+1 ]);
            Data[ i * Width + j]=sum;
        }*/
    Image Temp = (*this);
    std::vector <int> Distinct;
    for (int i = 0; i < Height; ++ i)
        for (int j = 0; j < Width; ++ j)
        {
            Distinct.push_back(abs(Data[i * Width + j]));
        }
    sort(Distinct.begin(), Distinct.end());
    int Cut1 = Distinct[Distinct.size() * 46 / 50];
    int Cut0 = Distinct[Distinct.size() / 800];
    qDebug("Cut0 = %d", Cut0);
    std::deque <int> Q;

    /*
    for (int i = 0; i < Height; ++ i)
        for (int j = 0; j < Width; ++ j) {
            if (abs(Data[i * Width + j]) - Cut1 >= 0) {
                Q.push_back(i * Width + j);
                Data[i * Width + j] = 255;
            }
            else {
                Data[i * Width + j] = 0;
            }
        }

    while (!Q.empty()) {
        int Cur = Q.front();
        Q.pop_front();
        for (int dx = - Width - Width; dx <= Width + Width; dx += Width)
            for (int dy = - 2; dy <= 2; ++ dy)
                if (Cur + dx + dy >= 0 && Cur + dx + dy < Width * Height &&
                        std::abs(Data[Cur + dx + dy]) != 255 && abs(Temp.Data[Cur + dx + dy] - Temp.Data[Cur]) <= 2) {
                    Q.push_back(Cur + dx + dy);
                    Data[Cur + dx + dy] = 255;
                }
    }
    */

    int ABox = 10;
    for (int _ = 0; _ < Width * Height / ABox; ++ _)
    {
        int tlx = rand() % (Height - ABox);
        int tly = rand() % (Width - ABox);
        int brx = tlx + ABox;
        int bry = tly + ABox;
        Distinct.clear();
        for (int i = tlx; i < brx; ++ i)
            for (int j = tly; j < bry; ++ j)
            {
                Distinct.push_back(abs(Temp.Data[i * Width + j]));
            }
        sort(Distinct.begin(), Distinct.end());
        Cut1 = Distinct[Distinct.size() * 19 / 20];
        for (int i = tlx; i < brx; ++ i)
            for (int j = tly; j < bry; ++ j)
            {
                if (abs(Temp.Data[i * Width + j]) - Cut1 >= 0 || abs(Data[i * Width + j]) == 255)
                {
                    Q.push_back(i * Width + j);
                    Data[i * Width + j] = 255;
                }
                else
                {
                    Data[i * Width + j] = 0;
                }
            }

        while (!Q.empty())
        {
            int Cur = Q.front();
            Q.pop_front();
            for (int dx = - Width; dx <= Width; dx += Width)
                for (int dy = - 1; dy <= 1; ++ dy)
                {
                    int nx = (Cur + dx + dy) / Width;
                    int ny = (Cur + dx + dy) % Width;
                    if (nx >= tlx && nx < brx && ny >= tly && ny < bry &&
                            std::abs(Data[Cur + dx + dy]) != 255 && abs(Temp.Data[Cur + dx + dy] - Temp.Data[Cur]) <= 2)
                    {
                        Q.push_back(Cur + dx + dy);
                        Data[Cur + dx + dy] = 255;
                    }
                }
        }
        for (int i = tlx; i < brx; ++ i)
            for (int j = tly; j < bry; ++ j)
            {
                if (std::abs(Data[i * Width + j]) != 255)
                    Data[i * Width + j] = std::max(abs(Temp.Data[i * Width + j]) - 255 + Cut1 + 10, 0.0);
            }
    }

/*
    for (int i = 0; i < Height; ++ i)
        for (int j = 0; j < Width; ++ j) {
            if (std::abs(Data[i * Width + j]) != 255)
                Data[i * Width + j] = std::max(abs(Temp.Data[i * Width + j]) - 255 + Cut1 + 10, 0.0);
        }*/
/*
    for (int i = 0; i < Height; ++ i)
        for (int j = 0; j < Width; ++ j) {
            if (abs(abs(Data[i * Width + j]) - Cut0) < abs(abs(Data[i * Width + j]) - Cut1))
                Data[i * Width + j] = 0;
            else
                Data[i * Width + j] = 255;
        }
*/
}

void Image::Denoise_1()
{
    Image Temp=(*this);
/*    double Mid = 0;
    for (int i = 1; i < Height - 1; i++)
        for (int j = 1; j < Width -1; j++)
            Mid += abs (Temp.Data[i * Width + j ]);
    Mid = Mid / (Height * Width);
    Mid = Mid / Const_Color;
    Mid = 150;
    qDebug()<<Mid;
*/
    for (int i = 1; i < Height - 1; i++)
        for (int j = 1; j < Width -1; j++)
        {
            //double a=abs (Temp.Data[(i-1) * Width + j-1 ]);
           // if (abs(a-0)>0.1 && abs(a-255)>0.1)
           //     qDebug()<<a;
            double sum = 0;
           // sum += abs (Temp.Data[(i-1) * Width + j-1 ]);
            sum -= abs (Temp.Data[(i-1) * Width + j ]);
          //  sum += abs (Temp.Data[(i-1) * Width + j+1 ]);
            sum -= abs (Temp.Data[i * Width + j-1 ]);
            sum += 5*abs (Temp.Data[i * Width + j ]);
            sum -= abs (Temp.Data[i * Width + j+1 ]);
          //  sum += abs (Temp.Data[(i+1) * Width + j-1 ]);
            sum -= abs (Temp.Data[(i+1) * Width + j ]);
           // sum += abs (Temp.Data[(i+1) * Width + j+1 ]);
            if (sum < 180 )
               Data[i * Width + j]=0;
               else Data[ i * Width + j]=255;
        }

}

void Image::Denoise_2()
{
    Image Temp=(*this);
    for (int i = 1; i < Height - 1; i++)
        for (int j = 1; j < Width -1; j++)
        {
            //double a=abs (Temp.Data[(i-1) * Width + j-1 ]);
           // if (abs(a-0)>0.1 && abs(a-255)>0.1)
           //     qDebug()<<a;
            double sum = 0;
            sum += abs (Temp.Data[(i-1) * Width + j-1 ]);
            sum += abs (Temp.Data[(i-1) * Width + j ]);
            sum += abs (Temp.Data[(i-1) * Width + j+1 ]);
            sum += abs (Temp.Data[i * Width + j-1 ]);
          //  sum += 5*abs (Temp.Data[i * Width + j ]);
            sum += abs (Temp.Data[i * Width + j+1 ]);
            sum += abs (Temp.Data[(i+1) * Width + j-1 ]);
            sum += abs (Temp.Data[(i+1) * Width + j ]);
            sum += abs (Temp.Data[(i+1) * Width + j+1 ]);
            sum /=255;

            if (sum <  2)
               Data[i * Width + j]=0;
            if (sum >  6)
               Data[ i * Width + j]=255;
        }

}

void Image::Denoise_3()
{
    Image Temp=(*this);
    for (int i = 1; i < Height - 1; i++)
        for (int j = 1; j < Width -1; j++)
        {
            //double a=abs (Temp.Data[(i-1) * Width + j-1 ]);
           // if (abs(a-0)>0.1 && abs(a-255)>0.1)
           //     qDebug()<<a;
            double sum = 0;
            sum += abs (Temp.Data[(i-1) * Width + j-1 ]);
            sum += abs (Temp.Data[(i-1) * Width + j ]);
            sum += abs (Temp.Data[(i-1) * Width + j+1 ]);
            sum += abs (Temp.Data[i * Width + j-1 ]);
          //  sum += 5*abs (Temp.Data[i * Width + j ]);
            sum += abs (Temp.Data[i * Width + j+1 ]);
            sum += abs (Temp.Data[(i+1) * Width + j-1 ]);
            sum += abs (Temp.Data[(i+1) * Width + j ]);
            sum += abs (Temp.Data[(i+1) * Width + j+1 ]);
            sum /=255;

         //   if (sum ==  0)
         //      Data[i * Width + j]=0;
            if (sum ==  8)
               Data[ i * Width + j]=255;
        }

}


/*
int Image::Get_Power() const{
    int Power=1,sum=1;
    while (sum<Width*Height){
        sum*=2;
        Power++;
    }
    return Power;
}
*/
