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


// WING GAME

void DrawCourse(int LR,int x0,int y0,int movex, int movey)
{
    
    // int sx = 5;
    // int sy = 5;
    if(LR == 0)
    {
        glColor3ub(0,0,0);
    }
    else
    {
        glColor3ub(255,0,0);
    }
    glBegin(GL_LINES);
    glVertex2i(x0,y0);
    // glVertex2i(x0+ movex,y0 + movey);
    glVertex2i(movex,movey);
    glEnd();
    // Right Line
}

void ProcessCollision()
{

}

double Abs(double a,double b)
{
    return sqrt((a-b)*(a-b));
}

void SetGravity(double level)
{

}

void DrawBoost()
{

}
// vector<double> Rotate(double cx,double cy,double x, double y, double angle)
// {
//     x = cx + (x-cx)*cos(angle*PI/180) - (y- cy)*sin(angle*PI/180);
//     y = cy + (x-cx)*sin(angle*PI/180) + (y- cy)*cos(angle*PI/180);
//     vector<double> rotation = {x,y};
//     return rotation;
// }

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
    srand(time(nullptr));
    const double H = 800;
    double cx = 60.0, cy = 20.0, vx = 0.0, vy = 0.0, acc = 1.2, len = 3.0;
    double angle = 0;
    double x1 = cx;
    double y1 = cy + len;
    double x2 = cx + len*cos(PI/6);
    double y2 = cy - len*sin(PI/6);
    double x3 = cx - len*cos(PI/6);
    double y3 = cy - len*sin(PI/6);
    // int x0 = 30, y0 = 10, movex = 5, movey = 4;
    // int iteration = 40;
    vector<vector<int>> leftLine {{30,5},{40,10},{60,15},
                                    {80,25},{85,30},{100,40},
                                    {90,55},{70,65},{60,70},
                                    {40,70},{20,65},{10,40},
                                    {8,30}};
    vector<vector<int>> rightLine {};
    for(auto i: leftLine)
    {
        rightLine.push_back({i[0]+15+ rand()%10,i[1]+10+rand()%5});
    }
                                    // {,},{,},{,},
                                    // {,},{,},{,}};
                                    
    FsOpenWindow(0,0,1200,H,1);
    for(;;)
    {
        FsPollDevice();
		auto key=FsInkey();
		if(FSKEY_ESC==key)
		{
			break;
		}
    
        // Moving >>
        if(FSKEY_LEFT==key)
        {
            angle-=3;
        }
        if(FSKEY_RIGHT == key)
        {
            angle -= 3;
        }
        // angle +=0.1;

        // x1 = cx + len*cos(angle*(PI/180)) - len*sin(angle*(PI/180));
        // y1 = cy + len*sin(angle*(PI/180)) + len*cos(angle*(PI/180));
        // x2 = cx + len*cos(angle*(PI/180)) - len*sin(angle*(PI/180));
        // y2 = cy + len*sin(angle*(PI/180)) + len*cos(angle*(PI/180));
        // x3 = cx + len*cos(angle*(PI/180)) - len*sin(angle*(PI/180));
        // y3 = cy + len*sin(angle*(PI/180)) + len*cos(angle*(PI/180));

        x1 = cx + (x1-cx)*cos(angle*(PI/180)) - (y1- cy)*sin(angle*(PI/180));
        y1 = cy + (x1-cx)*sin(angle*(PI/180)) + (y1- cy)*cos(angle*(PI/180));
        x2 = cx + (x2-cx)*cos(angle*(PI/180)) - (y2- cy)*sin(angle*(PI/180));
        y2 = cy + (x2-cx)*sin(angle*(PI/180)) + (y2- cy)*cos(angle*(PI/180));
        x3 = cx + (x3-cx)*cos(angle*(PI/180)) - (y3- cy)*sin(angle*(PI/180));
        y3 = cy + (x3-cx)*sin(angle*(PI/180)) + (y3- cy)*cos(angle*(PI/180));
        cout << cx << " "<< x2<< " " << x3 << "\n";
        
        if(angle>= 360 || angle <= -360)
        {
            angle = 0;
        }
        // if(0!=FsGetKeyState(FSKEY_SPACE))
        // {
        //     acc = -3;
        // }
        
        // Moving <<

        // vx += acc*sin(angle*PI/180)*dt;
        // vy += acc*cos(angle*PI/180)*dt;
        // cx += vx*dt;
        // cy += vy*dt; 


        // Drawing >>
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        for(int i= 0; i<leftLine.size()-1; ++i)
        {
            //Left Line
            DrawCourse(0,10*leftLine[i][0],H-10*leftLine[i][1],
                        10*leftLine[i+1][0],H-10*leftLine[i+1][1]);
            
            //Right Line
            DrawCourse(1,10*rightLine[i][0],H-10*rightLine[i][1],
                        10*rightLine[i+1][0],H-10*rightLine[i+1][1]);


            // if (i>iteration/2 ||(x0+movex)>120 ||(x0+movex) < 0)
            // {
            //     movex=-(3+rand()%4);
            // }
            // if (i>iteration/2 || (y0+movey)>80 ||(y0+movey) < 0)
            // {
            //     movey=-(2+rand()%3);
            // }
            // else
            // {
            //     movex=(3+rand()%4);
            //     movey=(2+rand()%3);
            // }
            // x0 = x0 + movex;
            // y0 = y0 + movey;
            
        }
        DrawWing(cx*10,H - cy*10,
                 x1*10,H - y1*10,
                 x2*10,H - y2*10,
                 x3*10,H - y3*10,
                 angle);
        glFlush();
        FsSwapBuffers();
        // Drawing <<

        FsSleep(25);
    }
    return 0;
}