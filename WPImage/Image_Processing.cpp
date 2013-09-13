#include "Image_Processing.h"

//public::
Image_Processing::Image_Processing():ima()
{
    Parallel_Angel = 0;
    Width = 0;
    Height = 0;
}

Image_Processing::Image_Processing(Image src):ima(src)
{
    Parallel_Angel = 0;
    Width = ima.Get_Width();
    Height = ima.Get_Height();
}

Image_Processing& Image_Processing::operator   =(const Image& src)
{
    ima = src;
    Width = src.Get_Width();
    Height = src.Get_Height();
    Parallel_Angel = 0;
    return *this;
}

Image_Processing& Image_Processing::operator   =(const Image_Processing& src)
{
    if (this == &src){
       return *this;
    }
    ima = src.ima;
    Width = src.Width;
    Height = src.Height;
    Parallel_Angel = 0;
    return *this;
}

Image Image_Processing::Get_Image()const
{
    return ima;
}


WPScore *Image_Processing::Save_Data(){
        qDebug()<<3;
    WPScore *file = new WPScore;
    file->lockForWrite();
    WPPart *p = file->newPart("yi'an");
    int now =0;
    std::map<int,int> chan;
    chan.clear();
    int pitch = 0;
    qDebug()<<int(Dat.size());

    while (now < int(Dat.size()) )
    {
        if (Dat[now] == -2) //一行结束
        { qDebug()<<6;
           chan.clear();
           now++; qDebug()<<6;
           continue;
        }
        if (Dat[now] == -1) //一个音符结束
        { qDebug()<<5;
           now++;qDebug()<<5;
           continue;
        }
        if (Dat[now] == 5){ // 读入音符

           int high = Dat[now+1];
           int len = Dat[now+2];
           qDebug()<<4<<" "<<high<<" "<<len;
           int fraca,fracb;
           Get_Frac(fraca, fracb, len);
           qDebug()<<4<<" "<<high<<" "<<len;
           Get_High(high,chan[high],pitch);
                      qDebug()<<4<<" "<<high<<" "<<fraca<<" "<<fracb;
           WPNote nt(high, Fraction (fraca, fracb));
           qDebug()<<4<<" "<<high<<" "<<fraca<<" "<<fracb;
           WPMultinote mnt(nt);
           qDebug()<<4<<" "<<high<<" "<<fraca<<" "<<fracb;
           p->insertMultinote(Fraction (1, 0), mnt);
           now++;
           now++;
           now++;
                   qDebug()<<4;
           continue;
        }

    }
        qDebug()<<3;
    file->unlock();
    return file;
}

void Image_Processing::Get_File(const Image &src)
{
    ima = src;
    Width = src.Get_Width();
    Height = src.Get_Height();
    Parallel_Angel = 0;
    return;
}

bool Image_Processing::Main_Process(const QString& aa)
{
    QImage image;
    if (!image.load(aa))
       return 0;

    Image src;
    src.Read_Image(image);

    Get_File(src);

    ima.Denoise();
    Find_Parallel_Angel();
    Find_Parallel_Lines();
    //TS();

    for (int i = 0; i < int (Lines.size()) / 5; i++)   //Maybe we need more alogirthm
    {
        Line_Process(Lines[i*5], Lines[i*5+1], Lines[i*5+2], Lines[i*5+3], Lines[i*5+4]);
    }

    freopen("a.txt","w",stdout);               //??
    printf("%d",44);                           //??
    int now = 0;
    now++;
    while (now < int(Dat.size()))
    {

        printf("%d \n",Dat[now]);
        now++;
    }

    fclose(stdout);
    return 1;
}



//private:

bool Image_Processing::Check_Color_Diff(const int& i,const int& j,const double& Ai,const double& Aj)const
{
    if (round(i+Ai)<0 || round(i+Ai)>=Height || round(j+Aj)<0 || round(j+Aj)>=Width)
       return 0;
    if (abs(round (ima.Get_Data(i, j)-ima.Get_Data(round(i + Ai), round(j + Aj))) )<Diff_Color)
       return 1;
    return 0;
}

double Image_Processing::Distance_Point(const int &xa, const int &ya, const int &xb, const int &yb) const
{
    double sum=(xa-xb)*(xa-xb)+(ya-yb)*(ya-yb);
    sum = std::sqrt(sum);
    return sum;
}

bool Image_Processing::Check_On_Lines(const int &i, const int &j, const Parallel_Lines &b1, const Parallel_Lines &b2,
                         const Parallel_Lines &b3, const Parallel_Lines &b4, const Parallel_Lines &b5)const
{
	double K = -tan(Parallel_Angel);

    Parallel_Lines b0 ;
    b0.L = b1.L - ( b5.L - b1.L ) / 4;
    b0.R = b1.R - ( b5.R - b1.R ) / 4;
    b0.B = b1.B - ( b5.B - b1.B ) / 4;

    Parallel_Lines b6 ;
    b6.L = b5.L + ( b5.L - b1.L ) / 4;
    b6.R = b5.R + ( b5.R - b1.R ) / 4;
    b6.B = b5.B + ( b5.B - b1.B ) / 4;

    Parallel_Lines b_1 ;
    b_1.L = b0.L - ( b5.L - b1.L ) / 4;
    b_1.R = b0.R - ( b5.R - b1.R ) / 4;
    b_1.B = b0.B - ( b5.B - b1.B ) / 4;

    Parallel_Lines b7 ;
    b7.L = b6.L + ( b5.L - b1.L ) / 4;
    b7.R = b6.R + ( b5.R - b1.R ) / 4;
    b7.B = b6.B + ( b5.B - b1.B ) / 4;

    if (i == round(j * K + b_1.B))
       return 1;
    if (i == round(j * K + b0.B))
       return 1;
    if (i == round(j * K + b1.B))
	   return 1;
	if (i == round(j * K + b2.B))
	   return 1;
	if (i == round(j * K + b3.B))
	   return 1;
	if (i == round(j * K + b4.B))
	   return 1;
	if (i == round(j * K + b5.B))
	   return 1;
    if (i == round(j * K + b6.B))
       return 1;
    if (i == round(j * K + b7.B))
       return 1;
	return 0;
}

bool Image_Processing::Check_On_Mid(const int &i, const int &j, const Parallel_Lines &b1, const Parallel_Lines &b2,
                         const Parallel_Lines &b3, const Parallel_Lines &b4, const Parallel_Lines &b5)const
{
    double K = -tan(Parallel_Angel);
    Parallel_Lines b0 ;
    b0.L = b1.L - ( b5.L - b1.L ) / 4;
    b0.R = b1.R - ( b5.R - b1.R ) / 4;
    b0.B = b1.B - ( b5.B - b1.B ) / 4;

    Parallel_Lines b6 ;
    b6.L = b5.L + ( b5.L - b1.L ) / 4;
    b6.R = b5.R + ( b5.R - b1.R ) / 4;
    b6.B = b5.B + ( b5.B - b1.B ) / 4;

    Parallel_Lines b_1 ;
    b_1.L = b0.L - ( b5.L - b1.L ) / 4;
    b_1.R = b0.R - ( b5.R - b1.R ) / 4;
    b_1.B = b0.B - ( b5.B - b1.B ) / 4;

    Parallel_Lines b7 ;
    b7.L = b6.L + ( b5.L - b1.L ) / 4;
    b7.R = b6.R + ( b5.R - b1.R ) / 4;
    b7.B = b6.B + ( b5.B - b1.B ) / 4;

    if (i == round(j * K + ( b_1.B + b0.B ) / 2 ))
       return 1;
    if (i == round(j * K + ( b0.B + b1.B ) / 2 ))
       return 1;
    if (i == round(j * K + ( b1.B + b2.B ) / 2 ))
       return 1;
    if (i == round(j * K + ( b2.B + b3.B ) / 2 ))
       return 1;
    if (i == round(j * K + ( b3.B + b4.B ) / 2 ))
       return 1;
    if (i == round(j * K + ( b4.B + b5.B ) / 2 ))
       return 1;
    if (i == round(j * K + ( b5.B + b6.B ) / 2 ))
       return 1;
    if (i == round(j * K + ( b6.B + b7.B ) / 2 ))
       return 1;

    return 0;
}

int Image_Processing::Get_Shine_On(const int & i, const int & j, const int & b) const
{
    double K=-tan(Parallel_Angel);
    double t = 1.0 * ( K * j + (-1) * i + b ) / sqrt( K * K + 1 );
    return round( - t * K + j);
}

void Image_Processing::Get_Frac(int &fraca, int &fracb, const int &len)
{
    fraca = 1;
    fracb = len%100;
    if (len > 100)
    {
       fraca = 3;
       fracb *= 2;
    }
}

void Image_Processing::Get_High(int &high, const int &ff,  const int & pitch)
{
    if (pitch == 0) // 高音
    {
        if (high == 28)
        {
           high = 15;
        }  else
        if (high == 26)
        {
           high = 14;
        }  else
        if (high == 24)
        {
           high = 12;
        }  else
        if (high == 22)
        {
           high = 10;
        }  else
        if (high == 20)
        {
           high = 8;
        }  else
        if (high == 18)
        {
           high = 7;
        }  else
        if (high == 16)
        {
           high = 5;
        }  else
        if (high == 14)
        {
           high = 3;
        }  else
        if (high == 12)
        {
           high = 2;
        }  else
        if (high == 10)
        {
           high = 0;
        }  else
        if (high == 8)
        {
           high = -2;
        }  else
        if (high == 6)
        {
           high = -4;
        }  else
        if (high == 4)
        {
           high = -5;
        }  else
        if (high == 2)
        {
           high = -7;
        }  else
        if (high == 0)
        {
           high = -9;
        }  else
        if (high == -2)
        {
           high = -10;
        }  else
        if (high == -4)
        {
           high = -12;
        }
    }
    high+=ff;
}

void Image_Processing::Find_Parallel_Angel()
{
     const double Pre_Angel = 1.0 * Pi / 180 / 4;
     std::map<int,int> f[161];
     double Ni[160],Nj[161];
     for (int i=0; i < 160; i++)
     {
         double Angel = (i -80) * Pre_Angel;
         Ni[i] = -sin(Angel);
         Nj[i] =  cos(Angel);
     }
     for (int i = 0; i < Height; i++)
         for (int j = 0; j < Width; j++)
             if (ima.Get_Data(i, j) < Black_Color)
                 for (int k = 0; k<160; k++)
                 {
                     int b = round(i - (Ni[k] / Nj[k]) * j);
                     f[k][b]++;
                 }
     int maxsum = 0, sum;
     std::map<int,int>::iterator iter;
     for (int i=0; i < 160; i++)
     {
         sum = 0;
         for (iter=f[i].begin(); iter != f[i].end(); iter++)
             if (iter->second > sum)
                sum = iter->second;
         if (sum > maxsum)
         {
            maxsum = sum;
            Parallel_Angel = (i -80) * Pre_Angel;
         }
     }
}

void Image_Processing::Find_Parallel_Angel_Easy()
{
    const double Pre_Angel = 1.0 * Pi / 180;
    int maxsum = 0 ,sum = 0;
    for (double angel=-35*Pre_Angel; angel<= 35*Pre_Angel; angel += Pre_Angel)
    {
        sum = 0;
        double Ni = -sin(angel);
        double Nj = cos(angel);
        for (int i = 0; i < Height; i++)
            for (int j = 0; j < Width; j++)
                if (ima.Get_Data(i, j) < Black_Color)
                {

                if (Check_Color_Diff(i, j, 3*Ni, 3*Nj))sum+=1;
                if (Check_Color_Diff(i, j, 11*Ni, 11*Nj))sum+=5;
                if (Check_Color_Diff(i, j, 29*Ni, 29*Nj))sum+=11;
            }
        if (sum > maxsum)
        {
           maxsum = sum;
           Parallel_Angel = angel;
        }
        //if (angel<0.001 && angel>-0.001){
        //    qDebug()<<Ni<<" "<<Nj;
       // }
      //  qDebug()<<sum<<" "<<angel;
    }
   // qDebug()<<Pre_Angel;
   // qDebug()<<Parallel_Angel;
}

void Image_Processing::Find_Parallel_Lines()
{
    std::map<int,int> L,R,Sum;
    double K = -tan(Parallel_Angel);//角度相反！！！！！！！！！！！！！
    for (int i = 0; i < Height; i++)
        for (int j = 0; j < Width; j++)
            if (ima.Get_Data(i, j) < Black_Color)
            {
               int B=round(i-K*j);
               L[B]=std::min(L[B],j);
               R[B]=std::max(R[B],j);
               Sum[B]++;
            }
    int Max_Sum = 0;
    std::map<int,int>::iterator iter;
    for (iter=Sum.begin();iter!=Sum.end();iter++)
        Max_Sum=std::max(Max_Sum,iter->second);
    Max_Sum = Max_Sum/WuXianPu;
    const int Min_B = -100000;
    int Last_B = Min_B, ge=0;
    for (iter = Sum.begin(); iter != Sum.end(); iter++)
        if (iter->second>Max_Sum)
        {
          //  qDebug()<<" a: "<<iter->first;
           if (Min_B==Last_B)//新的开始(以前没有)
           {
              Last_B=iter->first;
              continue;
           }
           if (Last_B+ge+1==iter->first)//未结束
           {
              ge++;
              continue;
           }
           //新的开始（以前有）
            Parallel_Lines ff;
            ff.B = (Last_B + ge / 2);
            ff.L = L[ff.B];
            ff.R = R[ff.B];
            Lines.push_back(ff);
            //qDebug()<< "f1";
            Last_B = iter->first;
            ge = 0;
        }
        else
        {
            if (Min_B==Last_B)
               continue;
            Parallel_Lines ff;
            ff.B = (Last_B + ge / 2);
            ff.L = L[ff.B];
            ff.R = R[ff.B];
            Lines.push_back(ff);
           // qDebug()<< "f2 "<<Last_B<< "  "<<ge;
            Last_B = Min_B;
            ge = 0;
        }
}

bool Image_Processing::Check_Black_Point(const int &ori_i, const int &ori_j,const int &L, const int &R, const Parallel_Lines &b1, const Parallel_Lines &b2,
                         const Parallel_Lines &b3, const Parallel_Lines &b4, const Parallel_Lines &b5)const
{
    double K = -tan(Parallel_Angel);
    Parallel_Lines b0 ;
    b0.L = b1.L - ( b5.L - b1.L ) / 4;
    b0.R = b1.R - ( b5.R - b1.R ) / 4;
    b0.B = b1.B - ( b5.B - b1.B ) / 4;

    Parallel_Lines b6 ;
    b6.L = b5.L + ( b5.L - b1.L ) / 4;
    b6.R = b5.R + ( b5.R - b1.R ) / 4;
    b6.B = b5.B + ( b5.B - b1.B ) / 4;

    Parallel_Lines b_1 ;
    b_1.L = b0.L - ( b5.L - b1.L ) / 4;
    b_1.R = b0.R - ( b5.R - b1.R ) / 4;
    b_1.B = b0.B - ( b5.B - b1.B ) / 4;

    Parallel_Lines b7 ;
    b7.L = b6.L + ( b5.L - b1.L ) / 4;
    b7.R = b6.R + ( b5.R - b1.R ) / 4;
    b7.B = b6.B + ( b5.B - b1.B ) / 4;

    double rad= double(( b5.B - b1.B )) / 8;
    int Mw,Ms,Ma,Md;
    Mw = round(std::min(K * b_1.L + b_1.B, K * b_1.R + b_1.B))-( b5.B - b1.B ) / 4;
    Ms = round(std::min(K * b7.L + b7.B, K * b7.R + b7.B))+( b5.B - b1.B ) / 4;
    Ma = L;
    Md = R;
    int Sum_Black=0, Sum_White=0;
   // qDebug()<< rad;
    for (int i = Mw; i <= Ms; i++)
        for (int j = Ma; j<= Md; j++)
            if (Distance_Point(i,j,ori_i,ori_j)<=rad)
                if (!Check_On_Lines(i,j,b1,b2,b3,b4,b5))
                {
                   if (ima.Get_Data(i,j)<Black_Color)
                      Sum_Black++;
                      else
                      Sum_White++;

                }

    if (double(Sum_White)/Sum_Black<0.11) return 1;
    return 0;
}

bool Image_Processing::Check_White_Point(const int &ori_i, const int &ori_j,const int &L, const int &R, const Parallel_Lines &b1, const Parallel_Lines &b2,
                         const Parallel_Lines &b3, const Parallel_Lines &b4, const Parallel_Lines &b5)const
{
    double K = -tan(Parallel_Angel);
    Parallel_Lines b0 , b6;
    b0.L = b1.L - ( b5.L - b1.L ) / 4;
    b0.R = b1.R - ( b5.R - b1.R ) / 4;
    b0.B = b1.B - ( b5.B - b1.B ) / 4;

    b6.L = b5.L + ( b5.L - b1.L ) / 4;
    b6.R = b5.R + ( b5.R - b1.R ) / 4;
    b6.B = b5.B + ( b5.B - b1.B ) / 4;

    double rad= double(( b5.B - b1.B )) / 8;
    int Mw,Ms,Ma,Md;
    Mw= round(std::min(K * b0.L + b0.B, K * b0.R + b0.B))-( b5.B - b1.B ) / 4;
    Ms= round(std::min(K * b6.L + b6.B, K * b6.R + b6.B))+( b5.B - b1.B ) / 4;
    Ma = L;
    Md = R;
    int Sum_Black=0, Sum_White=0;

    for (int i = Mw; i <= Ms; i++)
        for (int j = Ma; j<= Md; j++)
            if (Distance_Point(i,j,ori_i,ori_j)<=rad-2)
                if (!Check_On_Lines(i,j,b1,b2,b3,b4,b5))
                {
                   if (ima.Get_Data(i,j)<Black_Color)
                      Sum_Black++;
                      else Sum_White++;
                }
   // ima.Write_Data(ori_i,ori_j,100);

    if (double(Sum_Black)/Sum_White>0.15) return 0;
    Sum_Black=0;
    Sum_White=0;
    for (int i = Mw; i <= Ms; i++)
        for (int j = Ma; j<= Md; j++)
            if (Distance_Point(i,j,ori_i,ori_j)>= rad - 1 && Distance_Point(i,j,ori_i,ori_j)<= rad + 1)
            {
                if (!Check_On_Lines(i,j,b1,b2,b3,b4,b5))
                {
                   if (ima.Get_Data(i,j)<Black_Color)
                      Sum_Black++;
                      else Sum_White++;
                }
            }
    if (double(Sum_White)/Sum_Black<0.1)
       return 1;
    return 0;
}



void Image_Processing::Find_Point(const int &L, const int &R,
                                  std::vector<int> &Point_i, std::vector<int> &Point_j, std::vector<int> &Point_col,
          const Parallel_Lines &b1, const Parallel_Lines &b2, const Parallel_Lines &b3, const Parallel_Lines &b4, const Parallel_Lines &b5)
{
    double K = -tan(Parallel_Angel);
    Parallel_Lines b0;
    b0.L = round( b1.L - ( b5.L - b1.L ) * 0.75);
    b0.R = round( b1.R - ( b5.R - b1.R ) * 0.75);
    b0.B = round( b1.B - ( b5.B - b1.B ) * 0.75);
    Parallel_Lines b6;
    b6.L = round( b5.L + ( b5.L - b1.L ) * 0.75);
    b6.R = round( b5.R + ( b5.R - b1.R ) * 0.75);
    b6.B = round( b5.B + ( b5.B - b1.B ) * 0.75);
    int Mw,Ms;
    Mw= round(std::min(K * b0.L + b0.B, K * b0.R + b0.B));
    Ms= round(std::min(K * b6.L + b6.B, K * b6.R + b6.B));

    for (int j = L; j<= R; j++)
        for (int i = Mw; i <= Ms; i++)
            if (Check_On_Lines(i,j,b1,b2,b3,b4,b5) || Check_On_Mid(i,j,b1,b2,b3,b4,b5))
            {
                bool f1 = Check_Black_Point(i,j,L,R,b1,b2,b3,b4,b5); //is black point?
                bool f2 = Check_White_Point(i,j,L,R,b1,b2,b3,b4,b5); //is white point?
                if (f1)
                {
                    Point_i.push_back(i);
                    Point_j.push_back(j);
                    Point_col.push_back(1);
                    continue;
                }
                if (f2)
                {
                    Point_i.push_back(i);
                    Point_j.push_back(j);
                    Point_col.push_back(0);
                    qDebug()<<12345<<" "<<i<<" "<<j;
                    continue;
                }
            }
    std::vector<int> Temp_i=Point_i;
    std::vector<int> Temp_j=Point_j;
    std::vector<int> Temp_col=Point_col;
    Point_i.clear();
    Point_j.clear();
    Point_col.clear();
    //qDebug()<<" "<<int(Temp_i.size());
    double rad = double(( b5.B - b1.B )) / 8;
    for (int i =0;i<int( Temp_i.size());i++)
    {
        if (i!=0 && Distance_Point(Temp_i[i], Temp_j[i], Temp_i[i - 1], Temp_j[i - 1])<rad)
           continue;
        Point_i.push_back(Temp_i[i]);
        Point_j.push_back(Temp_j[i]);
        Point_col.push_back(Temp_col[i]);
    }
}

int Image_Processing::Check_Point_High(const int &i, const int &j, const Parallel_Lines &b1, const Parallel_Lines &b2,
                         const Parallel_Lines &b3, const Parallel_Lines &b4, const Parallel_Lines &b5)const
{
    if (!Check_On_Lines(i,j,b1,b2,b3,b4,b5) && !Check_On_Mid(i,j,b1,b2,b3,b4,b5))
       return -10000;


    double K = -tan(Parallel_Angel);
    Parallel_Lines b0 ;
    b0.L = b1.L - ( b5.L - b1.L ) / 4;
    b0.R = b1.R - ( b5.R - b1.R ) / 4;
    b0.B = b1.B - ( b5.B - b1.B ) / 4;

    Parallel_Lines b6 ;
    b6.L = b5.L + ( b5.L - b1.L ) / 4;
    b6.R = b5.R + ( b5.R - b1.R ) / 4;
    b6.B = b5.B + ( b5.B - b1.B ) / 4;

    Parallel_Lines b_1 ;
    b_1.L = b0.L - ( b5.L - b1.L ) / 4;
    b_1.R = b0.R - ( b5.R - b1.R ) / 4;
    b_1.B = b0.B - ( b5.B - b1.B ) / 4;

    Parallel_Lines b7 ;
    b7.L = b6.L + ( b5.L - b1.L ) / 4;
    b7.R = b6.R + ( b5.R - b1.R ) / 4;
    b7.B = b6.B + ( b5.B - b1.B ) / 4;

    if (i == round(j * K + b_1.B))
       return 28;
    if (i == round(j * K + b0.B))
       return 24;
    if (i == round(j * K + b1.B))
       return 20;
    if (i == round(j * K + b2.B))
       return 16;
    if (i == round(j * K + b3.B))
       return 12;
    if (i == round(j * K + b4.B))
       return 8;
    if (i == round(j * K + b5.B))
       return 4;
    if (i == round(j * K + b6.B))
       return 0;
    if (i == round(j * K + b7.B))
       return -4;

    if (i == round(j * K + ( b_1.B + b0.B ) / 2 ))
       return 26;
    if (i == round(j * K + ( b0.B + b1.B ) / 2 ))
       return 22;
    if (i == round(j * K + ( b1.B + b2.B ) / 2 ))
       return 18;
    if (i == round(j * K + ( b2.B + b3.B ) / 2 ))
       return 14;
    if (i == round(j * K + ( b3.B + b4.B ) / 2 ))
       return 10;
    if (i == round(j * K + ( b4.B + b5.B ) / 2 ))
       return 6;
    if (i == round(j * K + ( b5.B + b6.B ) / 2 ))
       return 2;
    if (i == round(j * K + ( b6.B + b7.B ) / 2 ))
       return -2;

    return -100000;
}





bool Image_Processing::Check_Need_Half(const int &ori_i, const int &ori_j, const Parallel_Lines &b1, const Parallel_Lines &b2,
                            const Parallel_Lines &b3, const Parallel_Lines &b4, const Parallel_Lines &b5)
{

    double rad= double(( b5.B - b1.B )) / 8;
  //  int next_pos =0;
    int Sum = 0;
    int No_Sum = 0;
  /*
         j-1  j   j+1  j+2  j+3 j+4
   i-1   *    *   *    *     *   *
   i     *    #   #              *
   i+1   *    #   #              *
   i+2   *                       *
   i+3   *                       *
   i+4   *    *   *    *     *   *
*/
    for (int i = ori_i - 2 * rad; i <= ori_i + 2 * rad; i++)
        for (int j = ori_j + int(1.5 * rad); j <= ori_j + int(3.5 * rad) ;j++)
        {
            Sum = 0;
            No_Sum = 0;
            if (!Check_On_Lines(i,j,b1,b2,b3,b4,b5) && ima.Get_Data(i, j)<Black_Color)
               Sum ++;
            if (!Check_On_Lines(i+1,j,b1,b2,b3,b4,b5) && ima.Get_Data(i+1, j)<Black_Color)
               Sum ++;
            if (!Check_On_Lines(i,j+1,b1,b2,b3,b4,b5) && ima.Get_Data(i, j+1)<Black_Color)
               Sum ++;
            if (!Check_On_Lines(i+1,j+1,b1,b2,b3,b4,b5) && ima.Get_Data( i+1, j+1)<Black_Color)
               Sum ++;


            if (Check_On_Lines(i-1,j-1,b1,b2,b3,b4,b5) || ima.Get_Data(i-1, j-1)>Black_Color)
               No_Sum ++;
            if (Check_On_Lines(i-1,j,b1,b2,b3,b4,b5) || ima.Get_Data(i-1, j)>Black_Color)
               No_Sum ++;
            if (Check_On_Lines(i-1,j+1,b1,b2,b3,b4,b5) || ima.Get_Data(i-1, j+1)>Black_Color)
               No_Sum ++;
            if (Check_On_Lines(i-1,j+2,b1,b2,b3,b4,b5) || ima.Get_Data(i-1, j+2)>Black_Color)
               No_Sum ++;
            if (Check_On_Lines(i-1,j+3,b1,b2,b3,b4,b5) || ima.Get_Data(i-1, j+3)>Black_Color)
               No_Sum ++;
            if (Check_On_Lines(i-1,j+4,b1,b2,b3,b4,b5) || ima.Get_Data(i-1, j+4)>Black_Color)
               No_Sum ++;


            if (Check_On_Lines(i,j-1,b1,b2,b3,b4,b5) || ima.Get_Data(i, j-1)>Black_Color)
               No_Sum ++;
            if (Check_On_Lines(i,j+4,b1,b2,b3,b4,b5) || ima.Get_Data(i, j+4)>Black_Color)
               No_Sum ++;
            if (Check_On_Lines(i+1,j-1,b1,b2,b3,b4,b5) || ima.Get_Data(i+1, j-1)>Black_Color)
               No_Sum ++;
            if (Check_On_Lines(i+1,j+4,b1,b2,b3,b4,b5) || ima.Get_Data(i+1, j+4)>Black_Color)
               No_Sum ++;
            if (Check_On_Lines(i+2,j-1,b1,b2,b3,b4,b5) || ima.Get_Data(i+2, j-1)>Black_Color)
               No_Sum ++;
            if (Check_On_Lines(i+2,j+4,b1,b2,b3,b4,b5) || ima.Get_Data(i+2, j+4)>Black_Color)
               No_Sum ++;
            if (Check_On_Lines(i+3,j-1,b1,b2,b3,b4,b5) || ima.Get_Data(i+3, j-1)>Black_Color)
               No_Sum ++;
            if (Check_On_Lines(i+3,j+4,b1,b2,b3,b4,b5) || ima.Get_Data(i+3, j+4)>Black_Color)
               No_Sum ++;


            if (Check_On_Lines(i+4,j-1,b1,b2,b3,b4,b5) || ima.Get_Data(i+4, j-1)>Black_Color)
               No_Sum ++;
            if (Check_On_Lines(i+4,j,b1,b2,b3,b4,b5) || ima.Get_Data(i+4, j)>Black_Color)
               No_Sum ++;
            if (Check_On_Lines(i+4,j+1,b1,b2,b3,b4,b5) || ima.Get_Data(i+4, j+1)>Black_Color)
               No_Sum ++;
            if (Check_On_Lines(i+4,j+2,b1,b2,b3,b4,b5) || ima.Get_Data(i+4, j+2)<Black_Color)
               No_Sum ++;
            if (Check_On_Lines(i+4,j+3,b1,b2,b3,b4,b5) || ima.Get_Data(i+4, j+3)>Black_Color)
               No_Sum ++;
            if (Check_On_Lines(i+4,j+4,b1,b2,b3,b4,b5) || ima.Get_Data(i+4, j+4)<Black_Color)
               No_Sum ++;

            if (Sum >= 3  && No_Sum > 15)
               return 1;
               //ima.Write_Data(i,j,100);
        }

    return 0;
}

int Image_Processing::Check_Point_Len(const int &ori_i, const int &ori_j,const int &high, const int &col, const int &L, const int &R, const Parallel_Lines &b1, const Parallel_Lines &b2,
                         const Parallel_Lines &b3, const Parallel_Lines &b4, const Parallel_Lines &b5)
{
    double K = -tan(Parallel_Angel);
    Parallel_Lines b0;
    b0.L = round( b1.L - ( b5.L - b1.L ) * 0.75);
    b0.R = round( b1.R - ( b5.R - b1.R ) * 0.75);
    b0.B = round( b1.B - ( b5.B - b1.B ) * 0.75);
    Parallel_Lines b6;
    b6.L = round( b5.L + ( b5.L - b1.L ) * 0.75);
    b6.R = round( b5.R + ( b5.R - b1.R ) * 0.75);
    b6.B = round( b5.B + ( b5.B - b1.B ) * 0.75);

    // ////////////////////////////////////////////////////////////////////Find longest line
    int Ma,Mw,Ms,Md;
    Ma= L;
    Mw= round(std::min(K * b0.L + b0.B, K * b0.R + b0.B)) - ( b5.B - b1.B ) / 4;;
    Md= R;
    Ms= round(std::min(K * b6.L + b6.B, K * b6.R + b6.B)) + ( b5.B - b1.B ) / 4;;
    int f[10000];
    std::fill(f, f+10000, 0 );
    for (int i = Mw; i <= Ms; i++)
        for (int j = Ma; j<= Md; j++)
            if (ima.Get_Data(i,j)<Black_Color && !Check_On_Lines(i,j,b1,b2,b3,b4,b5))
            {
                int Pos = Get_Shine_On(i, j, b1.B);
                if ( L <= Pos && Pos <= R )
                {
                    f[Pos]++;
                }
            }
    double rad= double(( b5.B - b1.B )) / 8;
    int Max_Sum = 0, Max_Pos;
    for (int i = L; i <= R ;i++)
        if (f[i] > Max_Sum)
        {
            Max_Sum = f[i];
            Max_Pos = i;
        }
    if (Max_Sum < 4 * rad)
    {
        if (col == 1) return 0;
       return 1;
    }

    // ////////////////////////////////////////////////////////////////////////Get high and low
    double Ki=-sin(Parallel_Angel - Pi /2);
    double Kj= cos(Parallel_Angel - Pi /2);

    double ti = K * Max_Pos + b0.B - ( b5.B - b1.B ) / 2;
    double tj = Max_Pos;
    int last = 0;
    int Max_Long, Max_up, Max_down;
    Max_Long = 0;
    while (ti<= K * tj + b5.B + ( b5.B - b1.B ) / 2)
    {
          ti+=Ki;
          tj+=Kj;
          if (Check_On_Lines(int(ti),int(tj),b1,b2,b3,b4,b5))
             continue;
          if (ima.Get_Data(int(ti),int(tj))<Black_Color)//is black
          {
              if (last == 0)
                  last = int(ti);
          }
          else{
              if (last != 0)
              {
                 int dis =int (ti) - last + 1;
                 if (dis>=Max_Long)
                 {
                    Max_Long = dis;
                    Max_up = last;
                    Max_down = int (ti);
                 }
                 last = 0;
              }
          }
    }


    // //////////////////////////////////////////////////////////////////////////get result
    std::vector<int> tol;
    ti = Max_up;
    tj = Max_Pos;
    while (ti <= Max_down)
    {
          ti+=Ki;
          tj+=Kj;
          if (Check_On_Lines(int(ti),int(tj),b1,b2,b3,b4,b5))
             continue;
          if (ima.Get_Data(int(ti-K*2),int(tj-2))<Black_Color
             || ima.Get_Data(int(ti+K*2),int(tj+2))<Black_Color)
          {//is black
              if (last == 0)
                  last = int(ti);
          }
          else{
              if (last != 0)
              {
                 int dis =int (ti) - last + 1;
                 if (dis > 1)
                    tol.push_back(dis);
                 last = 0;
              }
          }
    }
    if (last!=0)
    {
        int dis =Max_down - last + 1;
        if (dis > 1)
           tol.push_back(dis);
        last = 0;
    }

    std::sort(tol.begin(),tol.end());
    qDebug()<<high<<" "<<tol.size();
    if (tol.size()>1)
        qDebug()<<tol[0]<<" "<<tol[1];
    if (tol.size() == 0)
        return 0;
    int half = tol[int( tol.size() - 1) / 2];
    int res = 0;

    for (int i = 0; i < int(tol.size()); i++)
        if (tol[i]>=half / 2)
        {
           res++;
           if (tol[i]> half * 2)
              res++;
           if (tol[i]> half * 3)
              res++;
        }
    if (col == 0){
       return 2;
    }
    if (res >= 5)
       res = 64;
    if (res == 4)
       res = 32;
    if (res == 3)
       res = 16;
    if (res == 2)
       res = 8;
    if (res == 1)
       res = 4;
    if (res == 0)
       return 0;

    if (Check_Need_Half(ori_i,ori_j,b1,b2,b3,b4,b5))
       res+=100;
    return res;
    /*
    double Ki=-sin(Parallel_Angel - Pi /2);
    double Kj= cos(Parallel_Angel - Pi /2);
  //  qDebug() << Ki<<" asd "<<Kj;

    double ti = K * L + b0.B;
    double tj = L;
    while (ti<= K * tj + b5.B){
          ti+=Ki;
          tj+=Kj;
          ima.Write_Data(int(ti),int(tj),30);
    }

    ti = K * R + b0.B;
    tj = R;
    while (ti<= K * tj + b5.B){
          ti+=Ki;
          tj+=Kj;
          ima.Write_Data(int(ti),int(tj),200);
    }
*/

}


void Image_Processing::Find_Note(const int &L, const int &R, const int &ge, const Parallel_Lines &b1, const Parallel_Lines &b2,
                         const Parallel_Lines &b3, const Parallel_Lines &b4, const Parallel_Lines &b5){

    if ( R - L < 2 )//No use
    {
       return;
    }
    if (ge == 0)
    {
       //gao di yin fu
       return;
    }

   // /////////////////////////////////////////////////////////Find all Point!!
    std::vector<int> Point_i , Point_j,Point_col;
    Find_Point(L,R,Point_i,Point_j,Point_col,b1,b2,b3,b4,b5);

   // /////////////////////////////////////////////////////////Find point high!!
    std::vector<int> Point_h;//
    for (int i=0;i<int(Point_i.size());i++)
    {
        int HH = Check_Point_High(Point_i[i],Point_j[i],b1,b2,b3,b4,b5);
        Point_h.push_back(HH);
    }

   // /////////////////////////////////////////////////////////Find point length!!
    std::vector<int> Point_len;//

    for (int i=0;i<int(Point_i.size());i++)
    {
        int PL, PR;
        if (i == 0)
           PL = L;
        else PL = Point_j[i] - (Point_j[i] - Point_j[i - 1] ) / 2;
        if (i+1 == int(Point_i.size()))
           PR = R;
        else PR = Point_j[i] + (Point_j[i+1] - Point_j[ i ] ) / 2;

        int LEN = Check_Point_Len(Point_i[i],Point_j[i],Point_h[i],Point_col[i],PL,PR,b1,b2,b3,b4,b5);

        Point_len.push_back(LEN);
    }

    // /////////////////////////////////////////////////////////Write Data!
    for (int i =0;i<int(Point_i.size());i++)
    {
       // Dat.push_back(Point_i[i]);
       // Dat.push_back(Point_j[i]);
        if (Point_len[i] != 0)
        {
        Dat.push_back(5);
        Dat.push_back(Point_h[i]);
        Dat.push_back(Point_len[i]);
        }
    }
    Dat.push_back(-1);
}



void Image_Processing::Line_Process(const Parallel_Lines &b1, const Parallel_Lines &b2,
                         const Parallel_Lines &b3, const Parallel_Lines &b4,const Parallel_Lines &b5)
{
     Parallel_Lines b0;
     b0.L = round( b1.L - ( b5.L - b1.L ) * 0.75);
     b0.R = round( b1.R - ( b5.R - b1.R ) * 0.75);
     b0.B = round( b1.B - ( b5.B - b1.B ) * 0.75);
     Parallel_Lines b6;
     b6.L = round( b5.L + ( b5.L - b1.L ) * 0.75);
     b6.R = round( b5.R + ( b5.R - b1.R ) * 0.75);
     b6.B = round( b5.B + ( b5.B - b1.B ) * 0.75);

     double K = -tan(Parallel_Angel);
     int Ma,Mw,Ms,Md;

     Ma= round(std::min(b0.L, b6.L));
     Mw= round(std::min(K * b0.L + b0.B, K * b0.R + b0.B));
     Md= round(std::max(b0.R, b6.R));
     Ms= round(std::min(K * b6.L + b6.B, K * b6.R + b6.B));

     double L = b1.L;
     double R = b1.R;

     int f[10000];
     std::fill(f, f+10000, 0 );
     for (int i = Mw; i <= Ms; i++)
         for (int j = Ma; j<= Md; j++)
             if (ima.Get_Data(i,j)<Black_Color && !Check_On_Lines(i,j,b1,b2,b3,b4,b5))
             {
                 int Pos = Get_Shine_On(i, j, b1.B);
                 if ( L <= Pos && Pos <= R )
                 {
                     f[Pos]++;
                 }
             }

    // int Max_Sum=0;
    // for (int i = L; i <= R; i++)
    //	 Max_Sum=std::max(Max_Sum, f[i]);
     std::vector<int> aa,bb;
     int lastpos, inuse = 0;
     for (int i = L; i <= R; i++)
         if ( f[i] > 1 )
         {
             if (!inuse)
             {
                lastpos = i;
                inuse = 1;
             }
         }  else
         {
             if (inuse)
             {
                aa.push_back(lastpos-1);
                bb.push_back(i);
                inuse = 0;
             }
         }

     for (int i = 0; i < int(aa.size()); i++)
     {
         Find_Note(aa[i], bb[i], i+1, b1, b2, b3, b4, b5);
     }

     Dat.push_back(-2);
}


void Image_Processing::TS(int ff)
{
    double i,j;
    i=ff;
    j=0;  //      qDebug()<<2;
    while (round(i)>=0 && round(i)<Height && round(j)>=0 && round(j)<Width)
    {
        ima.Write_Data(round(i),round(j),200);
        i+=-sin(Parallel_Angel);
        j+=cos(Parallel_Angel);
        //qDebug()<<1;
    }
}


void Image_Processing::TS()
{
    for (int i=0;i<int(Lines.size());i++)
    {
        TS(Lines[i].B);
    }
}


/*
int Gcd(int x, int y){
    if (x == 0 || y == 0) return 0;
    int z = x % y;
    while (z != 0){
          x = y;
          y = z;
          z = x % y;
    }
    return y;
}
*/
