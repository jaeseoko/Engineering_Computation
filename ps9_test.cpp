#include <stdio.h>
#include "fssimplewindow.h"
#include <iostream>

class CharBitmap
{
protected:
    int wid,hei;
    char *dat;
public:
    CharBitmap();
    ~CharBitmap();
    void CleanUp(void);

	// Add copy constructor and copy-assignment operator
    CharBitmap(const CharBitmap &incoming); // COPY CONSTRUCTOR
    CharBitmap &operator=(const CharBitmap &incoming); // COPY OPERATOR
    void copy_from(const CharBitmap &incoming);
	void set(const char incoming[]);
	const char *c_str(void) const;

    int GetWidth(void) const;
    int GetHeight(void) const;
    void SetBitmap(int bmpWid,int bmpHei,char bmp[]);
    void SetPixel(int x,int y,char c);

    void Print(void) const;
    void Draw(void) const;
    char GetPixel(int x,int y) const;
};
const char *CharBitmap::c_str(void) const
{
	return (nullptr==dat ? "" : dat);
}
CharBitmap::CharBitmap()
{
    // Write this function.
    wid=0,hei=0;
    dat = nullptr;
    
}
CharBitmap::~CharBitmap()
{
    // Write this function.
    CleanUp();
}
void CharBitmap::CleanUp(void)
{
    // Write this function.
    if(nullptr!=dat) delete[] dat;
    dat = nullptr;
}
int CharBitmap::GetWidth(void) const
{
    return wid;
}
int CharBitmap::GetHeight(void) const
{
    return hei;
}

CharBitmap::CharBitmap(const CharBitmap &incoming)
{
    dat = nullptr;
    copy_from(incoming);
}
CharBitmap &CharBitmap::operator=(const CharBitmap &incoming)
{
    copy_from(incoming);
    return *this;
}
void CharBitmap::copy_from(const CharBitmap &incoming)
{
    wid = incoming.wid,hei=incoming.hei;
    set(incoming.c_str());
}

void CharBitmap::set(const char incoming[])
{
    if(incoming!=dat)
    {
        CleanUp();
        dat = new char[wid*hei];
        if(nullptr!=incoming)    
        {
            for(int i = 0; i<wid*hei; ++i)
            {   
                dat[i] = incoming[i];
            }
        }
    }
}

void CharBitmap::SetBitmap(int bmpWid,int bmpHei,char bmp[])
{
    // Write this function.
    wid = bmpWid, hei = bmpHei;
    dat = new char[wid*hei];
    for(int i = 0; i<wid*hei; ++i)
    {   
        dat[i] = bmp[i];
    }
}
void CharBitmap::SetPixel(int x,int y,char c)
{
    if(x>=0 && x<wid && y>=0 && y <hei)
    {
        dat[y*wid+x] = c;
    }
}
void CharBitmap::Print(void) const
{
    int x,y;
    for(y=0; y<hei; y++)
    {
        for(x=0; x<wid; x++)
        {
            printf("%c",GetPixel(x,y));
        }
        printf("\n");
    }
}

void CharBitmap::Draw(void) const
{
    // Write this function.
    for(int y=0; y<hei; ++y)
    {
        for(int x=0; x<wid; ++x)
        {
            if('1'==GetPixel(x,y))
            {
                for(int i =0; i<wid; ++i)
                {
                    for(int j=0; j<hei; ++j)
                    {
                        glColor3ub(0,0,0);
                        glBegin(GL_POINTS);
                        glVertex2i(wid*x+i,y*hei+j);
                    }
                }
            }
        }
    }
    glEnd();
}

char CharBitmap::GetPixel(int x,int y) const
{
    if(0<=x && x<wid && 0<=y && y<hei)
    {
        return dat[y*wid+x];
    }
    return 0;
}

int main(void)
{
    char pattern[]=
    {
        "....11....11...."
        "...1..1..1..1..."
        "...1.11..1.11..."
        "...1.11..1.11..."
        "...1.11..1.11..."
        "...1.11..1.11..."
        "...1.11..1.11..."
        "...1.11111.11..."
        "..1..........1.."
        ".1............1."
        "1..............1"
        "1..............1"
        "1..............1"
        "1......11......1"
        ".11..........11."
        "...1111111111..."
    };

    CharBitmap bmp;
    bmp.SetBitmap(16,16,pattern);
    bmp.SetPixel(4,10,'1');
    bmp.SetPixel(5,10,'1');
    bmp.SetPixel(4,11,'1');
    bmp.SetPixel(5,11,'1');
    bmp.SetPixel(10,10,'1');
    bmp.SetPixel(11,10,'1');
    bmp.SetPixel(10,11,'1');
    bmp.SetPixel(11,11,'1');
    // bmp.SetPixel(4242,5000,'1');
    bmp.Print();
    
    

    // Test Copy

    // CharBitmap tmp1(bmp);        // Copy constructor
    // CharBitmap tmp2 = tmp1;      // copy operator

    // std::cout <<"\n";
    // tmp2.Print();
    // std::cout <<"\n";
    // tmp1.Print();


    // Check Address

    // std::cout << "pointer address bmp, tmp1,tmp2 = \n";
    // std::cout << &bmp << &tmp1 << "," <<&tmp2 <<std::endl;
    

    FsOpenWindow(16,16,256,256,1);
    while(FSKEY_NULL==FsInkey())
    {
        FsPollDevice();

        glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
        // tmp1.Draw();
        // tmp2.Draw();
        bmp.Draw();
        FsSwapBuffers();

        FsSleep(10);
    }

    return 0;
}
