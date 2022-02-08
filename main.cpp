#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif
#include <tchar.h>
#include <windows.h>
#include<vector>
#include <iostream>
#include<cmath>
#include<math.h>
#include<fstream>
#include<sstream>

using namespace std; //to make combination between console and my window.


string data = "";

int ROUND(int x)
{
    return (int)(x+0.5);
}


void swapp (int &x1,int &y1,int &x2,int &y2)
{
    int temp =x1;
    x1=x2;
    x2=temp;
    temp =y1;
    y1=y2;
    y2=temp;
}


struct point
{
    int x,y;
    point(int x,int y):x(x),y(y) {}
    point() {}
};


static point _leftBottom;
static point _rightTop;
static point _rightBottom;
static point _leftTop;
////////////////////////////////////////////////////////////////////////
//////////////DDA Algorithm////////////////////////////////////////////
void DrawLine_DDA(HDC hdc, int x1,int y1,int x2,int y2,COLORREF c){
    int dx = x2-x1;
    int dy = y2-y1;
    double slope = (double)dy/(double)dx;
    if (abs(dx)>=abs(dy))
    {
        if (x1>x2)
            swapp(x1,y1,x2,y2);
        int x=x1;
        double y =y1;
        SetPixel(hdc, x1, y1, c);
        while (x<x2)
        {
            x++;
            y+=slope;
            SetPixel(hdc,x, y, c);
        }
    }
    else
    {
        if (y1>y2)
            swapp(x1,y1,x2,y2);
        int y=y1;
        double x =x1;
        SetPixel(hdc, x1, y1, c);
        while (y<y2)
        {
            y++;
            x+=1/slope;
            SetPixel(hdc,x, y, c);
        }
    }
}

/////////////////////////////////////////////////////////////////////
//////////////MIDPOINT ALGORITHM FOR LINE ///////////////////////////
void BresenhamLine(HDC hdc, int x1,int y1,int x2,int y2,COLORREF c){
    int dx=x2-x1;
    int  dy=y2-y1;
    if(abs(dx)>abs(dy))
    {
        if(dx<=0)
        {
            swap(x1,x2);
            swap(y1,y2);
            dx=-dx;
            dy=-dy;
        }
        if(dy>=0)
        {
            int x=x1;
            int y=y1;
            while(x<x2)
            {
                int d=(2*y+1-(2*y1))*dx-(2*(x+1-x1)*dy);
                if(d<=0)
                {
                    x++;
                    y++;
                    SetPixel(hdc,x,y,c);
                }
                else
                {
                    x++;
                    SetPixel(hdc,x,y,c);
                }
            }

        }
        else
        {
            int x=x1;
            int y=y1;
            while(x<x2)
            {
                int d=((((2*y)+1)-(2*y1))*dx)-(2*(x+1-x1)*abs(dy));
                if(d<=0)
                {
                    x++;
                    y--;
                    SetPixel(hdc,x,y,c);
                }
                else
                {

                    x++;
                    SetPixel(hdc,x,y,c);
                }
            }


        }
    }
    else //(abs(dy) > abs(dx)
    {
        if(dy<0)
        {
            swap(x1,x2);
            swap(y1,y2);
            dx=-dx;
            dy=-dy;
        }
        if(dx>=0)
        {
            int x=x1;
            int y=y1;
            while(y<y2)
            {
                int d=2*(((y+1)-y1)*dx)-((((2*x)+1)-(2*x1))*dy);
                if(d>=0)
                {
                    x++;
                    y++;
                    SetPixel(hdc,x,y,c);

                }
                else
                {
                    y++;
                    SetPixel(hdc,x,y,c);
                }
            }

        }
        else// dx<0
        {
            int x=x1;
            int y=y1;
            while(y<y2)
            {
                int d=2*(y+1-y1)*abs(dx)-(((2*x+1)-(2*x1))*dy);
                if(d>=0)
                {
                    y++;
                    x--;
                    SetPixel(hdc,x,y,c);
                }

                else
                {
                    y++;
                    SetPixel(hdc,x,y,c);
                }
            }}}
}
/////////////////////////////////////////////////////////////////////////
//////////////PARAMETRIC ALGORITHM///////////////////////////////////////
void DrawLine_Parametric(HDC hdc,int x1,int y1,int x2,int y2,COLORREF c){

    double dt=(double)1.0/std::max(abs(x2-x1),abs(y2-y1));
    for(double t = 0; t<=1; t+=dt)
    {
        double x=x1+t*(x2-x1);
        double y=y1+t*(y2-y1);
        SetPixel(hdc,ROUND(x),ROUND(y),c);
    }



}
/////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void Draw8point(HDC hdc,int xc,int yc,int x,int y,COLORREF c){
    SetPixel(hdc,xc+x,yc+y,c);
    SetPixel(hdc,xc+x,yc-y,c);
    SetPixel(hdc,xc-x,yc+y,c);
    SetPixel(hdc,xc-x,yc-y,c);
    SetPixel(hdc,xc+y,yc-x,c);
    SetPixel(hdc,xc-y,yc+x,c);
    SetPixel(hdc,xc-y,yc-x,c);
    SetPixel(hdc,xc+y,yc+x,c);


}
///////////////////////////////////////////////////////////////////////
////////////////////////Direct circle/////////////////////////////////
void DrawCircle_Direct(HDC hdc, int xc,int yc, int R, COLORREF c)
{
    double x=0;
    double y=R;
    Draw8point(hdc,xc,yc,R,0,c);
    while(x<y)
    {
        x++;
        y=std::sqrt(R*R-x*x);
        Draw8point(hdc,xc,yc,x,y,c);
    }
}
////////////////////////////////////////////////////////////////////
///////////////////Polar circle/////////////////////////////////////
void DrawCircle_polar(HDC hdc, int xc,int yc, int R, COLORREF color)
{
    double dtheta=1.0/R;
    double c= std::cos(dtheta);
    double s= std::sin(dtheta);
    double x=R;
    double y=0;
    Draw8point(hdc,xc,yc,R,0,color);
    while(x>y)
    {
        double x1=x*c-y*s;
        y=x*s+y*c;
        x=x1;
        Draw8point(hdc,xc,yc,ROUND(x),ROUND(y),color);
    }
}

/////////////////////////////////////////////////////////////////
void Draw_Midpoint_circle(HDC hdc, int xc , int yc ,int r ,COLORREF color){
    int x = 0;
    int y = r;
    int d = 1-r;
    Draw8point(hdc,xc,yc,x,y,color);
    while (x<y){
        if (d<0)
            d+= 2*x+3;
        else
        {
            d+= 2*(x-y)+5;
            y--;
        }
        x++;
        Draw8point(hdc,xc,yc,x,y,color);
    }
}
/////////////////////////////////////////////////////////////////////
/////////////////Midpoint circle modified ///////////////////////////
void DrawCircle_MidPoint_Modified(HDC hdc,int xc,int yc,int R,COLORREF color)
{
    int x=0;
    int y=R;
    int d=1-R;
    int d1=3;
    int d2=5-2*R;
    Draw8point(hdc,xc,yc,x,y,color);
    while(x<y)
    {
        if(d<0)
        {
            d+=d1;
            d1+=2;
            d2+=2;
        }
        else
        {
            d+=d2;
            d2+=4;
            d1+=2;
            y--;
        }
        x++;
        Draw8point(hdc,xc,yc,x,y,color);
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////filing quarter/////////////////////////////////////////////////
void Draw8pointFilling(HDC hdc,int xc,int yc,int x,int y,int quarter,COLORREF color)
{
    if(quarter==1)
    {
        SetPixel(hdc,xc+x,yc+y,color);
        SetPixel(hdc,xc-x,yc+y,color);
        SetPixel(hdc,xc+x,yc-y,color);
        SetPixel(hdc,xc-x,yc-y,color);
        SetPixel(hdc,xc+y,yc+x,color);
        SetPixel(hdc,xc-y,yc+x,color);
        SetPixel(hdc,xc+y,yc-x,color);
        SetPixel(hdc,xc-y,yc-x,color);
        DrawLine_DDA(hdc,xc,yc,xc+x,yc-y,color);
        DrawLine_DDA(hdc,xc,yc,xc+y,yc-x,color);
    }

    else if(quarter==2)
    {
        SetPixel(hdc,xc+x,yc+y,color);
        SetPixel(hdc,xc-x,yc+y,color);
        SetPixel(hdc,xc+x,yc-y,color);
        SetPixel(hdc,xc-x,yc-y,color);
        SetPixel(hdc,xc+y,yc+x,color);
        SetPixel(hdc,xc-y,yc+x,color);
        SetPixel(hdc,xc+y,yc-x,color);
        SetPixel(hdc,xc-y,yc-x,color);
        DrawLine_DDA(hdc,xc,yc,xc-x,yc-y,color);
        DrawLine_DDA(hdc,xc,yc,xc-y,yc-x,color);

    }

    else if(quarter==3)
    {
        SetPixel(hdc,xc+x,yc+y,color);
        SetPixel(hdc,xc-x,yc+y,color);
        SetPixel(hdc,xc+x,yc-y,color);
        SetPixel(hdc,xc-x,yc-y,color);
        SetPixel(hdc,xc+y,yc+x,color);
        SetPixel(hdc,xc-y,yc+x,color);
        SetPixel(hdc,xc+y,yc-x,color);
        SetPixel(hdc,xc-y,yc-x,color);
        DrawLine_DDA(hdc,xc,yc,xc-x,yc+y,color);
        DrawLine_DDA(hdc,xc,yc,xc-y,yc+x,color);

    }
    else if(quarter==4)
    {
        SetPixel(hdc,xc+x,yc+y,color);
        SetPixel(hdc,xc-x,yc+y,color);
        SetPixel(hdc,xc+x,yc-y,color);
        SetPixel(hdc,xc-x,yc-y,color);
        SetPixel(hdc,xc+y,yc+x,color);
        SetPixel(hdc,xc-y,yc+x,color);
        SetPixel(hdc,xc+y,yc-x,color);
        SetPixel(hdc,xc-y,yc-x,color);
        DrawLine_DDA(hdc,xc,yc,xc+x,yc+y,color);
        DrawLine_DDA(hdc,xc,yc,xc+y,yc+x,color);

    }
}

void DrawCircleFilling(HDC hdc,int xc,int yc,int R,int quarter,COLORREF color)
{
    double dtheta=1.0/R;
    double c=std::cos(dtheta);
    double s=std::sin(dtheta);
    double x=R;
    double y=0;
    Draw8pointFilling(hdc,xc,yc,R,0,quarter,color);
    while (x>y)
    {
        double x1=x*c-y*s;
        y=x*s+y*c;
        x=x1;
        Draw8pointFilling(hdc,xc,yc,ROUND(x),ROUND(y),quarter,color);
    }
}
///////////////////////////////////////////////////////////////////////////
///////////Direct Ellipse //////////////////////////////////////////////////
void draw4point (HDC hdc, int x, int y, int xc, int yc, COLORREF c )
{
    SetPixel(hdc, xc+x, yc+y, c);
    SetPixel(hdc, xc-x, yc+y, c);
    SetPixel(hdc, xc+x, yc-y, c);
    SetPixel(hdc, xc-x, yc-y, c);
}

void  directellipse (HDC hdc,int xc,int yc,int R,int r2,COLORREF c)
{
  double x=R;
    int y=0;

    draw4point( hdc, R, 0, xc, yc,  c);
    while(x>=0)
    {
        x-=0.01;
        y= abs(r2*sqrt(1-(pow(x,2)/pow(R,2))));
        draw4point( hdc,  x,  y,  xc, yc,  c);
    }
}
//////////////////////////////////////////////////////////////////////
////////////////Polar Ellipse ///////////////////////////////////////
void DrawEllipsePolar(HDC hdc,int xc,int yc,int r,int r2,COLORREF c)
{
    double dtheta=1.0/r;
    for(double theta=0.0; theta<6.28; theta+=dtheta)
    {
        double x=xc+r2*cos(theta);
        double y=yc+r*sin(theta);
        SetPixel(hdc,ROUND(x),ROUND(y),c);
    }
}
/////////////////////////////////////////////////////////////////////
/////////////////clipping////////////////////////////////////////////
union OutCode
{
    unsigned All:4;
    struct
    {
        unsigned left:1,top:1,right:1,bottom:1;
    };
};

OutCode GetOutCode(double x,double y,int xleft,int ytop,int xright,int ybottom)
{
    OutCode out;
    out.All=0;
    if(x<xleft)out.left=1;
    else if(x>xright)out.right=1;
    if(y<ytop)out.top=1;
    else if(y>ybottom)out.bottom=1;
    return out;
}




void VIntersect(double xs,double ys,double xe,double ye,int x,double *xi,double *yi)
{

    *xi=x;
    *yi=ys+(x-xs)*(ye-ys)/(xe-xs);
}


void HIntersect(double xs,double ys,double xe,double ye,int y,double *xi,double *yi)
{
    *yi=y;
    *xi=xs+(y-ys)*(xe-xs)/(ye-ys);
}


void CohenSuth(HDC hdc,int xs,int ys,int xe,int ye,int xleft,int ytop,int xright,int ybottom,COLORREF c)
{
    double x1=xs,y1=ys,x2=xe,y2=ye;
    OutCode out1=GetOutCode(x1,y1,xleft,ytop,xright,ybottom);
    OutCode out2=GetOutCode(x2,y2,xleft,ytop,xright,ybottom);

    while( (out1.All || out2.All) && !(out1.All & out2.All))
    {
        double xi,yi;
        if(out1.All)
        {
            if(out1.left)VIntersect(x1,y1,x2,y2,xleft,&xi,&yi);
            else if(out1.top)HIntersect(x1,y1,x2,y2,ytop,&xi,&yi);
            else if(out1.right)VIntersect(x1,y1,x2,y2,xright,&xi,&yi);
            else HIntersect(x1,y1,x2,y2,ybottom,&xi,&yi);
            x1=xi;
            y1=yi;
            out1=GetOutCode(x1,y1,xleft,ytop,xright,ybottom);
        }
        else
        {
            if(out2.left)VIntersect(x1,y1,x2,y2,xleft,&xi,&yi);
            else if(out2.top)HIntersect(x1,y1,x2,y2,ytop,&xi,&yi);
            else if(out2.right)VIntersect(x1,y1,x2,y2,xright,&xi,&yi);
            else HIntersect(x1,y1,x2,y2,ybottom,&xi,&yi);
            x2=xi;
            y2=yi;
            out2=GetOutCode(x2,y2,xleft,ytop,xright,ybottom);
        }
    }

    if(!out1.All && !out2.All)
    {
        BresenhamLine(hdc,x1,y1,x2,y2,c);
    }
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//***implement of Clipping point algorithm
void PointClipping(HDC hdc ,int x,int y,int xleft,int ytop,int xright,int ybottom,COLORREF color)
{
    if(x>=xleft && x<= xright && y>=ytop && y<=ybottom)
    SetPixel(hdc,x,y,color);
}


/////////////////////////////////////////////
////////////Save Points/////////////////////
struct Save_Point
{
    string Function_Name;
    int x1 =0,x2 =0;
    int y1 =0,y2 =0;
    int R =0,r2 =0;
    int Rc,Gc,Bc;
    int quarter;
    point _leftBottom = point(0,0);
    point _rightTop = point(0,0);
    point _rightBottom= point(0,0);
    point _leftTop = point(0,0);
    int xleft =0;
    int ytop =0;
    int xright =0;
    int ybottom =0;

    Save_Point(string Function_Name,int x1,int y1,int x2,int y2,int Rc,int Gc,int Bc)  //Line
    {
        this->Function_Name=Function_Name;
        this->x1=x1;
        this->x2=x2;
        this->y1=y1;
        this->y2=y2;
        this->Rc=Rc;
        this->Gc=Gc;
        this->Bc=Bc;
    }

    Save_Point(string Function_Name,int x1,int y1,int x2,int y2,int xleft,int ytop,int xright,int ybottom,int Rc,int Gc,int Bc)//clipping line
    {
        this->Function_Name=Function_Name;
        this->x1=x1;
        this->x2=x2;
        this->y1=y1;
        this->y2=y2;
        this->xleft=xleft;
        this->xright=xright;
        this->ytop=ytop;
        this->ybottom=ybottom;
        this->Rc=Rc;
        this->Gc=Gc;
        this->Bc=Bc;
    }

    Save_Point(string Function_Name,int x1,int y1,int R,int r2,int Rc,int Gc,int Bc,char e) //Ellipse
    {
        this->Function_Name=Function_Name;
        this->x1=x1;
        this->R=R;
        this->r2=r2;
        this->y1=y1;
        this->Rc=Rc;
        this->Gc=Gc;
        this->Bc=Bc;
    }
    Save_Point(string Function_Name,int x1,int y1,int R,int Rc,int Gc,int Bc) //Circle
    {
        this->Function_Name=Function_Name;
        this->x1=x1;
        this->R=R;
        this->y1=y1;
        this->Rc=Rc;
        this->Gc=Gc;
        this->Bc=Bc;
    }
    Save_Point(string Function_Name,int x1,int y1,int R,int quarter,int Rc,int Gc,int Bc,string f) //Filling
    {
        this->Function_Name=Function_Name;
        this->x1=x1;
        this->R=R;
        this->y1=y1;
        this->Rc=Rc;
        this->Gc=Gc;
        this->Bc=Bc;
        this->quarter=quarter;
    }

};



vector<Save_Point>Arr_Save_Point;
void Save()
{
    data = "";

    for (auto it=Arr_Save_Point.begin(); it!=Arr_Save_Point.end(); ++it)
    {
        /* Save Function of All Line */
        if(it->Function_Name=="DDLine")
        {
            data += it->Function_Name+ ","+to_string(it->x1)+","+to_string(it->y1)+","+to_string(it->x2)+","+to_string(it->y2)+","+to_string(it->Rc)+","+to_string(it->Gc)+","+to_string(it->Bc)+"\n";
        }
        else if(it->Function_Name=="DPLine")
        {
            data += it->Function_Name+ ","+to_string(it->x1)+","+to_string(it->y1)+","+to_string(it->x2)+","+to_string(it->y2)+","+to_string(it->Rc)+","+to_string(it->Gc)+","+to_string(it->Bc)+"\n";
        }
        else if(it->Function_Name=="DMLine")
        {
            data += it->Function_Name+ ","+to_string(it->x1)+","+to_string(it->y1)+","+to_string(it->x2)+","+to_string(it->y2)+","+to_string(it->Rc)+","+to_string(it->Gc)+","+to_string(it->Bc)+"\n";
        }


        /* save Function of ALL Circle */

        if(it->Function_Name=="DDCircle")
        {
            data += it->Function_Name+ ","+to_string(it->x1)+","+to_string(it->y1)+","+to_string(it->R)+","+to_string(it->Rc)+","+to_string(it->Gc)+","+to_string(it->Bc)+"\n";
        }
        else if (it->Function_Name=="DPCircle")
        {
            data += it->Function_Name+ ","+to_string(it->x1)+","+to_string(it->y1)+","+to_string(it->R)+","+to_string(it->Rc)+","+to_string(it->Gc)+","+to_string(it->Bc)+"\n";
        }
        else if (it->Function_Name=="DMCircle")
        {
            data += it->Function_Name+ ","+to_string(it->x1)+","+to_string(it->y1)+","+to_string(it->R)+","+to_string(it->Rc)+","+to_string(it->Gc)+","+to_string(it->Bc)+"\n";
        }


        /*save Function of All Filling */

        if(it->Function_Name=="DPCircleFilling")
        {
            data += it->Function_Name+ ","+to_string(it->x1)+","+to_string(it->y1)+","+to_string(it->R)+","+to_string(it->quarter)+","+to_string(it->Rc)+","+to_string(it->Gc)+","+to_string(it->Bc)+"\n";
        }


        /* save Function of ALL Ellipse */
        if(it->Function_Name=="DDEllipse")
        {
            data += it->Function_Name+ ","+to_string(it->x1)+","+to_string(it->y1)+","+to_string(it->R)+","+to_string(it->r2)+","+to_string(it->Rc)+","+to_string(it->Gc)+","+to_string(it->Bc)+"\n";
        }
        else if(it->Function_Name=="DPEllipse")
        {
            data += it->Function_Name+ ","+to_string(it->x1)+","+to_string(it->y1)+","+to_string(it->R)+","+to_string(it->r2)+","+to_string(it->Rc)+","+to_string(it->Gc)+","+to_string(it->Bc)+"\n";
        }

        /* save line clipped on rectangle using line*/
        if(it->Function_Name=="CohenSuth")
        {
            data += it->Function_Name+ ","+to_string(it->x1)+","+to_string(it->y1)+","+to_string(it->x2)+","+to_string(it->y2)+","+to_string(it->xleft)+","+to_string(it->ytop)+","+to_string(it->xright)+","+to_string(it->ybottom)+","+to_string(it->Rc)+","+to_string(it->Gc)+","+to_string(it->Bc)+"\n";
        }

    }
    ofstream myfile;
    myfile.open ("Graphics.txt");
    myfile << data;
    myfile.close();

}


//Load Points//
void Load(HDC hdc)
{
    string Line;
    ifstream LoadFile;
    LoadFile.open("Graphics.txt");
    if (!LoadFile)
    {
        cout << "Unable to open file";
        return;
    }
    while(getline(LoadFile,Line))
    {
        vector<string> Fun_Load;
        string buff ="";
        for (auto n:Line){
            if (n != ',')
                buff +=n;
            else if (n == ',' && buff != "")
            {
                Fun_Load.push_back(buff);
                buff ="";
            }
        }
        if(buff != "") Fun_Load.push_back(buff);

        /* Load Function of All Line */
        if(Fun_Load[0]=="DDLine")
        {
            //stoi() is a function to covert string to integer
            COLORREF c =RGB(stoi(Fun_Load[5]),stoi(Fun_Load[6]),stoi(Fun_Load[7])); //the index of colors
            DrawLine_DDA(hdc,stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[3]),stoi(Fun_Load[4]),c);
            Save_Point x(Fun_Load[0],stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[3]),stoi(Fun_Load[4]),stoi(Fun_Load[5]),stoi(Fun_Load[6]),stoi(Fun_Load[7]));
            Arr_Save_Point.push_back(x);


        }
        else if(Fun_Load[0]=="DPLine")
        {
            COLORREF c =RGB(stoi(Fun_Load[5]),stoi(Fun_Load[6]),stoi(Fun_Load[7]));
            DrawLine_Parametric(hdc,stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[3]),stoi(Fun_Load[4]),c);
            Save_Point x(Fun_Load[0],stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[3]),stoi(Fun_Load[4]),stoi(Fun_Load[5]),stoi(Fun_Load[6]),stoi(Fun_Load[7]));
            Arr_Save_Point.push_back(x);
        }
        else if(Fun_Load[0]=="DMLine")
        {
            COLORREF c =RGB(stoi(Fun_Load[5]),stoi(Fun_Load[6]),stoi(Fun_Load[7]));
            BresenhamLine(hdc,stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[3]),stoi(Fun_Load[4]),c);
            Save_Point x(Fun_Load[0],stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[3]),stoi(Fun_Load[4]),stoi(Fun_Load[5]),stoi(Fun_Load[6]),stoi(Fun_Load[7]));
            Arr_Save_Point.push_back(x);
        }


        /* Load Function of ALL Circle */

        if(Fun_Load[0]=="DDCircle")
        {
            COLORREF c =RGB(stoi(Fun_Load[4]),stoi(Fun_Load[5]),stoi(Fun_Load[6]));
            DrawCircle_Direct(hdc,stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[3]),c);
            Save_Point x(Fun_Load[0],stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[3]),stoi(Fun_Load[4]),stoi(Fun_Load[5]),stoi(Fun_Load[6]));
            Arr_Save_Point.push_back(x);
        }
        else if (Fun_Load[0]=="DPCircle")
        {
            COLORREF c =RGB(stoi(Fun_Load[4]),stoi(Fun_Load[5]),stoi(Fun_Load[6]));
            DrawCircle_polar(hdc,stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[3]),c);
            Save_Point x(Fun_Load[0],stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[3]),stoi(Fun_Load[4]),stoi(Fun_Load[5]),stoi(Fun_Load[6]));
            Arr_Save_Point.push_back(x);
        }
        else if (Fun_Load[0]=="DMCircle")
        {
            COLORREF c =RGB(stoi(Fun_Load[4]),stoi(Fun_Load[5]),stoi(Fun_Load[6]));
            Draw_Midpoint_circle(hdc,stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[3]),c);
            Save_Point x(Fun_Load[0],stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[3]),stoi(Fun_Load[4]),stoi(Fun_Load[5]),stoi(Fun_Load[6]));
            Arr_Save_Point.push_back(x);
        }
        else if (Fun_Load[0] == "DMMCircle"){
            COLORREF c =RGB(stoi(Fun_Load[4]),stoi(Fun_Load[5]),stoi(Fun_Load[6]));
            DrawCircle_MidPoint_Modified(hdc,stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[3]),c);
            Save_Point x(Fun_Load[0],stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[3]),stoi(Fun_Load[4]),stoi(Fun_Load[5]),stoi(Fun_Load[6]));
            Arr_Save_Point.push_back(x);

        }


        /*Load Function of All Filling */
        if(Fun_Load[0]=="DPCircleFilling")
        {
            COLORREF c =RGB(stoi(Fun_Load[5]),stoi(Fun_Load[6]),stoi(Fun_Load[7]));
            DrawCircleFilling(hdc,stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[3]),stoi(Fun_Load[4]),c);
            Save_Point x(Fun_Load[0],stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[3]),stoi(Fun_Load[4]),stoi(Fun_Load[5]),stoi(Fun_Load[6]),stoi(Fun_Load[7]),"e");
            Arr_Save_Point.push_back(x);
        }

        /* Load Function of ALL Ellipse */

        if(Fun_Load[0]=="DDEllipse")
        {
            COLORREF c =RGB(stoi(Fun_Load[5]),stoi(Fun_Load[6]),stoi(Fun_Load[7]));
            directellipse(hdc,stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[3]),stoi(Fun_Load[4]),c);
            Save_Point x(Fun_Load[0],stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[3]),stoi(Fun_Load[4]),stoi(Fun_Load[5]),stoi(Fun_Load[6]),stoi(Fun_Load[7]),'e');
            Arr_Save_Point.push_back(x);
        }
        else if(Fun_Load[0]=="DPEllipse")
        {
            COLORREF c =RGB(stoi(Fun_Load[5]),stoi(Fun_Load[6]),stoi(Fun_Load[7]));
            DrawEllipsePolar(hdc,stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[3]),stoi(Fun_Load[4]),c);
            Save_Point x(Fun_Load[0],stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[3]),stoi(Fun_Load[4]),stoi(Fun_Load[5]),stoi(Fun_Load[6]),stoi(Fun_Load[7]),'e');
            Arr_Save_Point.push_back(x);
        }

        /* Load Function of Draw Rectangle */
         if(Fun_Load[0]=="CohenSuth")
        {
            COLORREF c =RGB(stoi(Fun_Load[9]),stoi(Fun_Load[10]),stoi(Fun_Load[11]));
            CohenSuth(hdc,stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[3]),stoi(Fun_Load[4]),stoi(Fun_Load[5]),stoi(Fun_Load[6]),stoi(Fun_Load[7]),stoi(Fun_Load[8]),c);
            Save_Point x(Fun_Load[0],stoi(Fun_Load[1]),stoi(Fun_Load[2]),stoi(Fun_Load[3]),stoi(Fun_Load[4]),stoi(Fun_Load[5]),stoi(Fun_Load[6]),stoi(Fun_Load[7]),stoi(Fun_Load[8]),stoi(Fun_Load[9]),stoi(Fun_Load[10]),stoi(Fun_Load[11]));
            Arr_Save_Point.push_back(x);
        }
    }
}


//Clean Points//
void Clear ()
{
    Arr_Save_Point.clear();
    //InvalidateRect(hWnd, NULL, TRUE);
}



/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("Graphics_Project");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_HAND); //change the shape of mouse to hand instead of arrow
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
     wincl.hbrBackground = (HBRUSH) COLOR_WINDOW; // to make the background white

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Graphics project App"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           544,                 /* The programs width */
           375,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */
HMENU hMenu;
void addMenu(HWND hwnd)
{
    hMenu = CreateMenu();

    HMENU hFileMenu = CreateMenu();
    AppendMenu(hFileMenu,MF_STRING,0,"Save");
    AppendMenu(hFileMenu,MF_STRING,1,"Load");
    AppendMenu(hFileMenu,MF_STRING,25,"Clean");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hFileMenu,"File");

    HMENU hLineMenu = CreateMenu();
    AppendMenu(hLineMenu,MF_STRING,2,"DDA");
    AppendMenu(hLineMenu,MF_STRING,3,"Mid Point");
    AppendMenu(hLineMenu,MF_STRING,4,"Parametric");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hLineMenu,"Line");

    HMENU hEllipseMenu = CreateMenu();
    AppendMenu(hEllipseMenu,MF_STRING,5,"Direct");
    AppendMenu(hEllipseMenu,MF_STRING,6,"Polar");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hEllipseMenu,"Ellipse");

    HMENU hCircleMenu = CreateMenu();
    AppendMenu(hCircleMenu,MF_STRING,7,"Direct,");
    AppendMenu(hCircleMenu,MF_STRING,8,"Polar iterative Polar");
    AppendMenu(hCircleMenu,MF_STRING,9,"Midpoint");
    AppendMenu(hCircleMenu,MF_STRING,26,"Modified MidPoint");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hCircleMenu,"Circle");


    HMENU hColorMenu = CreateMenu();
    AppendMenu(hColorMenu,MF_STRING,10,"Black");
    AppendMenu(hColorMenu,MF_STRING,11,"Red");
    AppendMenu(hColorMenu,MF_STRING,12,"Blue");
    AppendMenu(hColorMenu,MF_STRING,13,"Green");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hColorMenu,"Colors");

    HMENU hFilling = CreateMenu();
    AppendMenu(hFilling,MF_STRING,14,"Black");
    AppendMenu(hFilling,MF_STRING,15,"Red");
    AppendMenu(hFilling,MF_STRING,16,"Blue");
    AppendMenu(hFilling,MF_STRING,17,"Green");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hFilling,"Fill Circle");


    HMENU hquarter = CreateMenu();
    AppendMenu(hquarter,MF_STRING,18,"First");
    AppendMenu(hquarter,MF_STRING,19,"Second");
    AppendMenu(hquarter,MF_STRING,20,"Third");
    AppendMenu(hquarter,MF_STRING,21,"Fourth");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hquarter,"Quarter Circle");

    HMENU hwindow = CreateMenu();
    AppendMenu(hwindow,MF_STRING,22,"Clipping by Line");
    AppendMenu(hwindow,MF_STRING,23," Clipping by point");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hwindow,"Clipping");

    SetMenu(hwnd,hMenu);
}
int Rc,Gc,Bc;
COLORREF c = 0;
COLORREF cF = 0;
int x1,x2,x3;
int y_1,y2,y3;
int r,r2;
int R;
int quarter;
int m;
int X_start,X_end,Y_start,Y_end,X_left,Y_top,X_right,Y_bottom;
int Num_of_Points = 0;
int counter = 0;
int counter_ell =0;
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc = GetDC(hwnd);
    switch (message)                  /* handle the messages */
    {
    case WM_CREATE:
        addMenu(hwnd);
        break;
    case WM_COMMAND:
        {
            switch(wParam){
            case(0):
                cout<< "Saving Process\n\n" ;
                Save();
                cout<< "Save Done ! \n\n";
                break;
            case(1):
                cout<<"Load process\n\n";
                Load(hdc);
                cout<<"load Done !\n\n";
                break;
            case(2):
                m = 2;
                cout << "Draw Line by DDA\n\n";
                break;
            case(3):
                m=3;
                cout << "Draw Line by Mid Point\n\n";
                break;
            case(4):
                m =4;
                cout << "Draw Line by Parametric\n\n";
                break;
            case (5):
                m = 5;
                cout<<" Draw Direct ellipse \n\n";
                break;
            case(6):
                m =6;
                cout<<"Draw polar ellipse \n\n";
                break;
            case (7):
                m=7;
                cout<<"Draw direct circle \n\n";
                break;
            case(8):
                m =8;
                cout<<"Draw Polar circle \n\n";
                break;
            case(9):
                m = 9;
                cout<<"Draw Mid Point circle \n\n";
                break;
            case (26):
                m = 26;
                cout<<"Draw circle by Modified Mid point \n\n";
                break;
            case(10):
                cout<<"Draw by black color \n\n";
                c = 0;
                break;
            case (11):
                cout<<"Draw by red color \n\n";
                c = RGB(255,0,0);
                break;
            case(12):
                cout<<"Draw by blue color \n\n";
                c = RGB(0,0,255);
                break;
            case (13):
                cout<<"Draw by green color \n\n";
                c = RGB(0,255,0);
                break;
            case(14):
                m= 14;
                cout << "Fill circle by the Black color\n\n";
                cF = 0;
                break;
            case(15):
                m = 15;
                cout << "Fill circle by the Red color\n\n";
                cF = RGB(255,0,0);
                break;
            case (16):
                m = 16;
                cout<<"Fill circle by  the blue color \n\n";
                cF = RGB(0,0,255);
                break;
            case(17):
                m =17;
                cout<<"Fill circle by the green color \n\n";
                cF = RGB(0,255,0);
                break;
            case (18):
                m = 18;
                cout<<"Filling Circle with First Quarter \n\n";
                quarter=1;
                break;
            case (19):
                m = 19;
                cout<<"Filling circle with second Quarter \n\n";
                quarter = 2;
                break;
            case (20):
                m = 20;
                cout<<"Filling circle with third Quarter \n\n";
                quarter =3;
                break;
            case (21):
                m =21;
                cout<<"Filling circle with fourth Quarter \n\n";
                quarter = 4;
                break;
            case (22):
                m = 22;
                cout<<"line after clipping \n\n";
                break;
            case (23):
                m = 23;
                cout<<"clipping point \n\n";
                break;
            case(25):
                cout <<"Clean the window \n";
                Clear();
                InvalidateRect(hwnd, NULL, TRUE);
                break;

            }
        }
        break;

        case WM_LBUTTONDOWN:
              {
                x1 = LOWORD(lParam);
                y_1 = HIWORD(lParam);
                if (m == 22) //Line clipping
                {
                if(Num_of_Points == 0)
                {
                    X_left=LOWORD(lParam);
                    Y_top=HIWORD(lParam);
                    Num_of_Points++;
                }
                else if (Num_of_Points == 1){
                    X_right=LOWORD(lParam);
                    Y_bottom=HIWORD(lParam);
                    Rectangle(hdc, X_left,Y_top,X_right,Y_bottom);
                    Num_of_Points++;
                }
                else if(Num_of_Points==2)
                {
                    X_start=LOWORD(lParam);
                    Y_start=HIWORD(lParam);
                    Num_of_Points++;
                }
                else if (Num_of_Points==3)
                {
                X_end=LOWORD(lParam);
                Y_end=HIWORD(lParam);
                CohenSuth(hdc,X_start,Y_start,X_end,Y_end,X_left,Y_top,X_right,Y_bottom,c);
                Num_of_Points=2;
                }
                Save_Point x("CohenSuth",X_start,Y_start,X_end,Y_end,X_left,Y_top,X_right,Y_bottom,0,0,0);
                Arr_Save_Point.push_back(x);
                }

                else if (m == 23) //clipping point
                {
                    c = RGB(0,255,0);

                    if(counter == 0)
                {
                    X_left=LOWORD(lParam);
                    Y_top=HIWORD(lParam);
                    counter++;
                }
                else if (counter == 1){
                    X_right=LOWORD(lParam);
                    Y_bottom=HIWORD(lParam);
                    Rectangle(hdc, X_left,Y_top,X_right,Y_bottom);
                    counter++;
                }
                else if(counter==2)
                {
                    X_start=LOWORD(lParam);
                    Y_start=HIWORD(lParam);
                    counter++;
                }
                else if (counter==3)
                {
                    X_end=LOWORD(lParam);
                    Y_end=HIWORD(lParam);
                    PointClipping(hdc,X_end,Y_end,X_left,Y_top,X_right,Y_bottom,c);
                }
                }
               //////////////////////////////////////////////////////
                if(m == 5 ||m == 6) //Draw ellipse (Direct-polar)
                {
                    if (m==5){
                    if(counter_ell ==0){
                    x2 = LOWORD(lParam);
                    y2 = HIWORD(lParam);
                    counter_ell ++;
                    }
                    else if (counter_ell==1){
                    x3 = LOWORD(lParam);
                    y3 = HIWORD(lParam);
                    counter_ell++;
                    }
                    else if (counter_ell ==2){
                    r = ROUND(sqrt(pow(x2-x1,2.0)+pow(y2-y_1,2.0)));
                    r2 = ROUND(sqrt(pow(x3-x1,2.0)+pow(y3-y_1,2.0)));
                     cout<< "Draw Direct Ellipse "<< endl;
                     directellipse(hdc,x1,y_1,r,r2,c);
                     counter_ell =0;
                    }

                    if(c==RGB(255,0,0))
                    {
                        Save_Point x("DDEllipse",x1,y_1,r,r2,255,0,0,'e');
                        Arr_Save_Point.push_back(x);
                    }
                    else if (c==RGB(0,255,0))
                    {
                        Save_Point x("DDEllipse",x1,y_1,r,r2,0,255,0,'e');
                    Arr_Save_Point.push_back(x);
                    }
                    else if (c==RGB(0,0,255))
                    {
                    Save_Point x("DDEllipse",x1,y_1,r,r2,0,0,255,'e');
                    Arr_Save_Point.push_back(x);
                    }
                    else if (c==RGB(0,0,0))
                    {
                    Save_Point x("DDEllipse",x1,y_1,r,r2,0,0,0,'e');
                    Arr_Save_Point.push_back(x);
                    }
                    else
                    {
                    Save_Point x("DDEllipse",x1,y_1,r,r2,Rc,Gc,Bc,'e');
                    Arr_Save_Point.push_back(x);
                    }

                    cout<<"Draw Direct Ellipse Done !"<<endl;
                    }

                else if(m == 6)//Polar Ellipse
                {
                    if(counter_ell ==0){
                    x2 = LOWORD(lParam);
                    y2 = HIWORD(lParam);
                    counter_ell ++;
                    }
                    else if (counter_ell==1){
                    x3 = LOWORD(lParam);
                    y3 = HIWORD(lParam);
                    counter_ell++;
                    }
                    else if (counter_ell ==2){
                    r = ROUND(sqrt(pow(x2-x1,2.0)+pow(y2-y_1,2.0)));
                    r2 = ROUND(sqrt(pow(x3-x1,2.0)+pow(y3-y_1,2.0)));
                     cout<< "Draw polar Ellipse "<< endl;
                     DrawEllipsePolar( hdc, x1, y_1, r, r2, c);
                     counter_ell =0;
                    }

                if(c==RGB(255,0,0))
                {
                Save_Point x("DPEllipse",x1,y_1,r,r2,255,0,0,'e');
                Arr_Save_Point.push_back(x);
                }
                else if (c==RGB(0,255,0))
                {
                Save_Point x("DPEllipse",x1,y_1,r,r2,0,255,0,'e');
                Arr_Save_Point.push_back(x);
                }
                else if (c==RGB(0,0,255))
                {
                Save_Point x("DPEllipse",x1,y_1,r,r2,0,0,255,'e');
                Arr_Save_Point.push_back(x);
                 }
                else if (c==RGB(0,0,0))
                {
                Save_Point x("DPEllipse",x1,y_1,r,r2,0,0,0,'e');
                Arr_Save_Point.push_back(x);
                }
                else
                {
                Save_Point x("DPEllipse",x1,y_1,r,r2,Rc,Gc,Bc,'e');
                Arr_Save_Point.push_back(x);
                }
            cout<<"Draw Polar Ellipse Done !"<<endl;

                }
            }
                }
              break;

        case WM_RBUTTONDOWN:
            x2 = LOWORD(lParam);
            y2 = HIWORD(lParam);

            if (m == 7 || m == 8 || m == 9 || m == 26){
             R=ROUND(std::sqrt(std::pow((x2-x1),2.0)+pow((y2-y_1),2.0)));
            if(m == 7) //Circle(Direct)
            {
                cout<< "Draw Direct Circle"<< endl;
                DrawCircle_Direct(hdc,x1,y_1,R,c);
                if(c==RGB(255,0,0))
                {
                    Save_Point x("DDCircle",x1,y_1,R,255,0,0);
                    Arr_Save_Point.push_back(x);
                }
                else if (c==RGB(0,255,0))
                {

                    Save_Point x("DDCircle",x1,y_1,R,0,255,0);
                    Arr_Save_Point.push_back(x);
                }
                else if (c==RGB(0,0,255))
                {

                    Save_Point x("DDCircle",x1,y_1,R,0,0,255);
                    Arr_Save_Point.push_back(x);
                }
                else if (c==RGB(0,0,0))
                {

                    Save_Point x("DDCircle",x1,y_1,R,0,0,0);
                    Arr_Save_Point.push_back(x);
                }
                else
                {

                    Save_Point x("DDCircle",x1,y_1,R,Rc,Gc,Bc);
                    Arr_Save_Point.push_back(x);
                }
                cout<<"Draw Direct Circle Done !"<<endl;
            }

            else if (m == 8) //polar circle
                {
                cout<< "Draw Polar Circle"<< endl;
                DrawCircle_polar(hdc,x1,y_1,R,c);
                if(c==RGB(255,0,0))
                {
                    Save_Point x("DPCircle",x1,y_1,R,255,0,0);
                    Arr_Save_Point.push_back(x);
                }
                else if (c==RGB(0,255,0))
                {

                    Save_Point x("DPCircle",x1,y_1,R,0,255,0);
                    Arr_Save_Point.push_back(x);
                }
                else if (c==RGB(0,0,255))
                {

                    Save_Point x("DPCircle",x1,y_1,R,0,0,255);
                    Arr_Save_Point.push_back(x);
                }
                else if (c==RGB(0,0,0))
                {

                    Save_Point x("DPCircle",x1,y_1,R,0,0,0);
                    Arr_Save_Point.push_back(x);
                }
                else
                {

                    Save_Point x("DPCircle",x1,y_1,R,Rc,Gc,Bc);
                    Arr_Save_Point.push_back(x);
                }
                cout<<"Draw Polar Circle Done !"<<endl;
            }

             else if(m == 9)//Circle(MidPooint)
            {
                cout<< "Draw MidPoint Circle "<< endl;
                Draw_Midpoint_circle(hdc,x1,y_1,R,c);
                if(c==RGB(255,0,0))
                {
                    Save_Point x("DMCircle",x1,y_1,R,255,0,0);
                    Arr_Save_Point.push_back(x);
                }
                else if (c==RGB(0,255,0))
                {

                    Save_Point x("DMCircle",x1,y_1,R,0,255,0);
                    Arr_Save_Point.push_back(x);
                }
                else if (c==RGB(0,0,255))
                {

                    Save_Point x("DMCircle",x1,y_1,R,0,0,255);
                    Arr_Save_Point.push_back(x);
                }
                else if (c==RGB(0,0,0))
                {
                    Save_Point x("DMCircle",x1,y_1,R,0,0,0);
                    Arr_Save_Point.push_back(x);
                }
                else
                {
                    Save_Point x("DMCircle",x1,y_1,R,Rc,Gc,Bc);
                    Arr_Save_Point.push_back(x);
                }
                cout<<"Draw MidPoint Circle Done !"<<endl;
            }
            else if (m == 26 ) //Modified midpoint for circle
            {
                cout<< "Draw Modified MidPoint Circle "<< endl;
                DrawCircle_MidPoint_Modified(hdc,x1,y_1,R,c);
                if(c==RGB(255,0,0))
                {
                    Save_Point x("DMCircle",x1,y_1,R,255,0,0);
                    Arr_Save_Point.push_back(x);
                }
                else if (c==RGB(0,255,0))
                {

                    Save_Point x("DMCircle",x1,y_1,R,0,255,0);
                    Arr_Save_Point.push_back(x);
                }
                else if (c==RGB(0,0,255))
                {

                    Save_Point x("DMCircle",x1,y_1,R,0,0,255);
                    Arr_Save_Point.push_back(x);
                }
                else if (c==RGB(0,0,0))
                {
                    Save_Point x("DMCircle",x1,y_1,R,0,0,0);
                    Arr_Save_Point.push_back(x);
                }
                else
                {
                    Save_Point x("DMCircle",x1,y_1,R,Rc,Gc,Bc);
                    Arr_Save_Point.push_back(x);
                }
                cout<<"Draw Modified MidPoint Circle Done !"<<endl;

            }

            }

            else if (m == 14 || m == 15 || m == 16 || m ==17 ) //Filling Circle
            {
            x2 = LOWORD(lParam);
            y2 = HIWORD(lParam);
            R=ROUND(std::sqrt(std::pow(x2-x1,2.0)+pow(y2-y_1,2.0)));
            if(m == 14)//Filling Circle(Black color)
            {

                cout<< "Filling  Circle with black Color"<< endl;
                DrawCircleFilling(hdc,x1,y_1,R,quarter,RGB(0,0,0));
                Save_Point x("DPCircleFilling",x1,y_1,R,quarter,0,0,0,"e");
                Arr_Save_Point.push_back(x);
                cout<< "Filling  Circle with black Color Done"<< endl;


            }
            else if (m == 15)//Filling Circle(Red color)
            {
                cout<< "Filling  Circle with Red Color"<< endl;
                DrawCircleFilling(hdc,x1,y_1,R,quarter,RGB(255,0,0));
                Save_Point x("DPCircleFilling",x1,y_1,R,quarter,255,0,0,"e");
                Arr_Save_Point.push_back(x);
                cout<< "Filling  Circle with Red Color Done"<< endl;
            }
            else if (m == 16)//Filling Circle(Blue color)
            {
                cout<< "Filling  Circle with Blue Color"<< endl;
                DrawCircleFilling(hdc,x1,y_1,R,quarter,RGB(0,0,255));
                Save_Point x("DPCircleFilling",x1,y_1,R,quarter,0,0,255,"e");
                Arr_Save_Point.push_back(x);
                cout<< "Filling  Circle with Blue Color Done"<< endl;
            }
            else if (m == 17) //Filling Circle(Green color)
            {
                cout<< "Filling  Circle with Green Color"<< endl;
                DrawCircleFilling(hdc,x1,y_1,R,quarter,RGB(0,255,0));
                Save_Point x("DPCircleFilling",x1,y_1,R,quarter,0,255,0,"e");
                Arr_Save_Point.push_back(x);
                cout<< "Filling  Circle with Green Color Done"<< endl;
            }

        }



            else if (m == 2) //DDA Line
            {
                cout<< "Draw DDA line "<< endl;
                DrawLine_DDA(hdc,x1,y_1,x2,y2,c);
                if(c==RGB(255,0,0))
                {
                    Save_Point x("DDLine",x1,y_1,x2,y2,255,0,0);
                    Arr_Save_Point.push_back(x);
                }
                else if (c==RGB(0,255,0))
                {
                    Save_Point x("DDLine",x1,y_1,x2,y2,0,255,0);
                    Arr_Save_Point.push_back(x);
                }
                else if (c==RGB(0,0,255))
                {

                    Save_Point x("DDLine",x1,y_1,x2,y2,0,0,255);
                    Arr_Save_Point.push_back(x);
                }
                else if (c==RGB(0,0,0))
                {
                    Save_Point x("DDLine",x1,y_1,x2,y2,0,0,0);
                    Arr_Save_Point.push_back(x);
                }
                else
                {Save_Point x("DDLine",x1,y_1,x2,y2,Rc,Gc,Bc);
                    Arr_Save_Point.push_back(x);
                }
                cout<< "Draw DDA line Done !"<< endl;
            }
            else if (m == 3) //Midpoint Line
            {cout<< "Draw MidPoint Line "<< endl;
                BresenhamLine(hdc,x1,y_1,x2,y2,c);
                if(c==RGB(255,0,0))
                {
                    Save_Point x("DMLine",x1,y_1,x2,y2,255,0,0);
                    Arr_Save_Point.push_back(x);
                }
                else if (c==RGB(0,255,0))
                {
                    Save_Point x("DMLine",x1,y_1,x2,y2,0,255,0);
                    Arr_Save_Point.push_back(x);
                }
                else if (c==RGB(0,0,255))
                {
                    Save_Point x("DMLine",x1,y_1,x2,y2,0,0,255);
                    Arr_Save_Point.push_back(x);
                }
                else if (c==RGB(0,0,0))
                {Save_Point x("DMLine",x1,y_1,x2,y2,0,0,0);
                    Arr_Save_Point.push_back(x);
                }
                else
                {Save_Point x("DMLine",x1,y_1,x2,y2,Rc,Gc,Bc);
                    Arr_Save_Point.push_back(x);
                }
                cout<< "Draw mid point line Done !"<< endl;
            }

            else if (m == 4) //parametric Line
            {
                cout<< "Draw parametric Line"<< endl;
                DrawLine_Parametric(hdc,x1,y_1,x2,y2,c);
                if(c==RGB(255,0,0))
                {

                    Save_Point x("DPLine",x1,y_1,x2,y2,255,0,0);
                    Arr_Save_Point.push_back(x);
                }
                else if (c==RGB(0,255,0))
                {

                    Save_Point x("DPLine",x1,y_1,x2,y2,0,255,0);
                    Arr_Save_Point.push_back(x);
                }
                else if (c==RGB(0,0,255))
                {

                    Save_Point x("DPLine",x1,y_1,x2,y2,0,0,255);
                    Arr_Save_Point.push_back(x);
                }
                else if (c==RGB(0,0,0))
                {

                    Save_Point x("DPLine",x1,y_1,x2,y2,0,0,0);
                    Arr_Save_Point.push_back(x);
                }
                else
                {

                    Save_Point x("DPLine",x1,y_1,x2,y2,Rc,Gc,Bc);
                    Arr_Save_Point.push_back(x);
                }
                cout<<"Draw parametric line Done !"<< endl;
            }

                break;

        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
