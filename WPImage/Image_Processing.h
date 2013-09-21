#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H
#include "Image.h"
#include "Parallel_Lines.h"
#include <QDebug>
#include <vector>
#include "WPScore/WPScore.h"
#include <QString>
//#include <iostream>

class Image_Processing
{
public:
    Image_Processing();

    Image_Processing(Image);

    Image_Processing& operator =(const Image_Processing&);//

    Image_Processing& operator =(const Image&);//

    Image Get_Image()const;//OK!

    bool Main_Process(const QString&);

    WPScore *Save_Data();

    void Get_File(const Image&);

private:

    bool Check_Color_Diff(const int&, const int&, const double&, const double&)const;

    double Distance_Point(const int&, const int &, const int&, const int &)const;

    bool Check_On_Lines(const int&, const int &, const Parallel_Lines &, const Parallel_Lines &, const Parallel_Lines &,
                      const Parallel_Lines &,const Parallel_Lines &)const;

    bool Check_On_Mid(const int&, const int &, const Parallel_Lines &, const Parallel_Lines &, const Parallel_Lines &,
                      const Parallel_Lines &,const Parallel_Lines &)const;

    int Get_Shine_On(const int&, const int &, const int&)const;

    void Get_Frac(int&, int&, const int &);

    void Get_High(int&, const int &,  const int &);

    void Find_Parallel_Angel();

    void Find_Parallel_Angel_Easy();//Not recommend

    void Find_Parallel_Lines();

    bool Check_Black_Point(const int&, const int &, const int&, const int &, const Parallel_Lines &, const Parallel_Lines &, const Parallel_Lines &,
                           const Parallel_Lines &,const Parallel_Lines &)const;

    bool Check_White_Point(const int&, const int &, const int&, const int &, const Parallel_Lines &, const Parallel_Lines &, const Parallel_Lines &,
                           const Parallel_Lines &,const Parallel_Lines &)const;

    void Find_Point(const int&, const int&, std::vector<int>&, std::vector<int>&, std::vector<int>& ,
                    const Parallel_Lines &, const Parallel_Lines &, const Parallel_Lines &, const Parallel_Lines &,const Parallel_Lines &);

    int Check_Point_High(const int&, const int &, const Parallel_Lines &, const Parallel_Lines &, const Parallel_Lines &,
                      const Parallel_Lines &,const Parallel_Lines &)const;







    bool Check_Need_Half(const int&, const int &, const Parallel_Lines &, const Parallel_Lines &, const Parallel_Lines &,
                      const Parallel_Lines &,const Parallel_Lines &);

    int Check_Point_Len(const int&, const int &, const int&, const int &, const int&, const int&, const Parallel_Lines &, const Parallel_Lines &, const Parallel_Lines &,
                      const Parallel_Lines &,const Parallel_Lines &);





    void Find_Note(const int&, const int&,  const int&, const Parallel_Lines &, const Parallel_Lines &, const Parallel_Lines &,
                   const Parallel_Lines &,const Parallel_Lines &);

    void Line_Process(const Parallel_Lines &, const Parallel_Lines &, const Parallel_Lines &,
                      const Parallel_Lines &,const Parallel_Lines &);


    void TS(int);//仅仅用来调试！！！！！！！！！！！！！！！！！！！
    void TS();//仅仅用来调试！！！！！！！！！！！！！！！！！！！
//    int Gcd(int,int);

private:
	static const int Diff_Color=2;
	static const int Black_Color=2;
	static const int WuXianPu=3;
  //  static const int WuxianPu_Color=200;
	static const double Pi=3.1415926;
    double Parallel_Angel;
    int Width;
    int Height;
    std::vector<Parallel_Lines> Lines;
	std::vector<int> Dat;
	Image ima;
};

#endif // IMAGE_PROCESSING_H
