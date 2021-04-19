#include <stdio.h>
#include "fssimplewindow.h"
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
using namespace std;
const double PI = 3.1415927;
const double dt =0.025;
const double G = 9.8;

void pop_front(vector<double> &v)
{
    v.erase(v.begin());
}
// wing 2
void DrawCourse(vector<double> x,vector<double> up,vector<double> down)
{
    //upper
    int k = 0;
    glColor3ub(0,0,0);
    glBegin(GL_LINE_STRIP);
    for(int i=0; i<x.size(); ++i)
    {
        glVertex2i(10*x[i],800 - 10*up[i]);
    }
    glEnd();
    //lower
    glColor3ub(255,0,0);
    glBegin(GL_LINE_STRIP);
    for(int i=0; i<x.size(); ++i)
    {
        glVertex2i(10*x[i],800 - 10*down[i]);
    }
    glEnd();
}

void DrawWing(double cx, double cy,
            double x1, double y1,
            double x2, double y2,
            double x3, double y3,
            double angle)
{   
    glColor3ub(0,0,255);
    glBegin(GL_POLYGON);
    glVertex2d(x1,y1);
    glVertex2d(x2,y2);
    glVertex2d(cx,cy);
    glVertex2d(x3,y3);
    glEnd(); 
}

int main(void)
{
    double courseH = 30;
    int boostState = 0;
    int x1=0, x2=0;
    double vel = 10;
    int courseLen = 120;
    
    vector<double> wallUp{};
    vector<double> wallDown{};
    time_t start,end;
    srand(time(nullptr));
    FsOpenWindow(0,courseLen*10,1200,800,1);
    time(&start);
    for(;;)
    {
        
        
        FsPollDevice();
		auto key=FsInkey();
		if(FSKEY_ESC==key)
		{
			break;
		}
        // int i = 0;
        // i++;
        //Drawing >>
        //get y points
        // for(int i=0; i<points.size()
        vector<double> points = {0,5,10,15,20,25,30,35,40,45,50,55
                            ,60,65,70,75,80,85,90,95,100,105,110,115,120,125,130}; // x points
        double y1 = 40 + rand()%10;
        double y2 = y1 - courseH;
        wallUp.push_back(y1);
        wallDown.push_back(y2);
        
        
            
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        DrawCourse(points,wallUp,wallDown);
        glFlush();
        FsSwapBuffers();
        // Drawing <<

        FsSleep(90);
        time(&end);
        int elapsed = end - start;
        cout << elapsed <<"\n";

        if(wallUp.size()>=points.size() && elapsed % 1 == 0)
        {
            // cout << "CHECK" << "\n";
            pop_front(wallUp);
            pop_front(wallDown);
            wallUp.pop_back();
            wallDown.pop_back();
        }
    }
    
    return 0;
}