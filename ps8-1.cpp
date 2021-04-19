#include <stdio.h>
#include "fssimplewindow.h"
#include <iostream>
using namespace std;

void DrawPixel(int x,int y)
{
    int w = 16;
    glColor3ub(0,0,0);
    glBegin(GL_QUADS);
    glVertex2i(x,y);
    glVertex2i(x,y+w);
    glVertex2i(x+w,y+w);
    glVertex2i(x+w,y);
    glEnd();
}


int main(void)
{
    // Character for the cell (x,y) will be pattern[y*16+x]
    char pattern[]=
    {
        // By the way, if you want to define a long string, you can write like this. 
        "...1........1..."
        "...11......11..."
        "...1.1....1.1..."
        "...1..1..1..1..." 
        "...1..1111..1..." 
        "..1..........1.." 
        "..1..11..11..1.." 
        "..1..........1.." 
        ".1..111..111..1." 
        ".1.1.1.11.1.1.1." 
        "1..1.1.11.1.1..1" 
        "1...111..111...1" 
        ".1............1." 
        ".1.....11.....1." 
        "..111......111.." 
        ".....111111....."
    };
    FsOpenWindow(16,16,256,256,0); 
     
    // Fill your code here.
    int x=0,y=0,count = 0;
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    for(auto i: pattern)
    {
        count++;
        if(i=='1')
        {
            DrawPixel(x,y);
            x+=16;
        }
        else if(i =='.')
        {
            x+=16;
        }
        if(count%16 == 0)
        {
            x = 0;
            y+= 16;
        }
        else if(count > 255)
        {
            break;
        }
    }
    glFlush();

    FsPollDevice();
    while(FSKEY_NULL==FsInkey())
    {
        FsPollDevice();
        FsSleep(25);
    }
    
    return 0;
}