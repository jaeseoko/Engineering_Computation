#include <stdio.h>
#include "fssimplewindow.h"
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

const double PI = 3.1415927;
const int nObstacles = 5;
const double dt =0.025;
const double G =9.8;

bool CheckCollision(int mx,int my,int tx,int ty,int tw,int th)
{
    int dx = sqrt((tx - mx)*(tx - mx));
    int dy = sqrt((ty - my)*(ty - my));
	if(dx<=tw/2 && dy<=th/2)
	{
		return true;
	}
	return false;
}
void DrawCannon(double angle,int x,int y)
{
    int w = 10; // width and height of the rectangular cannon
    int l = 15; // length of the cannon aim
    glColor3ub(0,0,255);
    glBegin(GL_QUADS);
    glVertex2i(x - w/2,  y + w/2);
    glVertex2i(x + w/2,  y + w/2);
    glVertex2i(x + w/2,  y - w/2);
    glVertex2i(x - w/2,  y - w/2);
    glEnd();
    
    glColor3ub(0,0,200);
    glBegin(GL_LINES);
    glVertex2i(50,550);
    glVertex2i(50 + l*cos(angle*PI/180),550 - l*sin(angle*PI/180));
    glEnd();
}
void DrawCannonBall(double cx,double cy,double rad,int fill)
{
	glColor3ub(255,0,0);
    if(0!=fill)
    {
        glBegin(GL_POLYGON);
    }
    else
    {
        glBegin(GL_LINE_LOOP);
    }
	for(int i=0; i<64; i++)
    {
        double angle=(double)i*PI/32.0;
        double x=(double)cx+cos(angle)*(double)rad;
        double y=(double)cy+sin(angle)*(double)rad;
        glVertex2i(x,y);
    }
	glEnd();
}

void DrawTarget(double x, double y,double tw, double th)
{
    glColor3ub(255,0,0);
    glBegin(GL_QUADS);
    glVertex2i(x - tw/2, y + th/2);
    glVertex2i(x + tw/2, y + th/2);
    glVertex2i(x + tw/2, y - th/2);
    glVertex2i(x - tw/2, y - th/2);
    glEnd();
}

void DrawObstacle(int ox,int oy, int ow,int oh, int fill)
{  
    glColor3ub(0,255,0);
    if(0!=fill)
    {
        glBegin(GL_QUADS);
    }
    else
    {
        glBegin(GL_LINE_LOOP);
    }
    
    glVertex2i(ox - ow/2, oy + oh/2);
    glVertex2i(ox + ow/2, oy + oh/2);
    glVertex2i(ox + ow/2, oy - oh/2);
    glVertex2i(ox - ow/2, oy - oh/2);
    glEnd();
}
void DrawTrajectory(double btx1, double bty1, double btx2, double bty2)
{
    glColor3ub(0,0,0);
    glBegin(GL_LINES);
    glVertex2d(btx1,bty1);
    glVertex2d(btx2,bty2);
    glEnd();
}

int main(void)
{
    double tx = 75.0, ty = 60.0, tw = 5.0, th = 5.0;
    int tState=1;
    int cx = 50, cy = 550;
    double bx=5.0, by=5.0;
    double brad = 2.0, bvel = 40.0;
    int bState = 0;
    double vx = 0.0, vy = 0.0;
    double angle = 30.0;
    int ow[nObstacles],oh[nObstacles],ox[nObstacles],oy[nObstacles],oState[nObstacles],oFill[nObstacles];
    std::vector<double> btx{}, bty{};
    srand(time(nullptr));
    for(int i=0; i<nObstacles; ++i)
        {
            ow[i] = 8 + rand()%8;
            oh[i] = 8 + rand()%8;
            ox[i] = 10 + rand()%60;
            oy[i] = 10 + rand()%40;
            oFill[i] = 1;
            oState[i] = 1;
        }

    FsOpenWindow(16,16,800,600,1);
    for(;;)
    {
        FsPollDevice();
        auto key=FsInkey();
        if(FSKEY_ESC==key)
        {
            break;
        }
        // Moving >>
        angle += 1;
        if(angle>90)
        {
            angle = 0;
        }

        ty -= 0.25;
        if(ty<=0)
        {
            ty = 60;
        }
        for(int i=0; i<nObstacles; ++i)
        {
            if(0!=oState[i] && true==CheckCollision(bx,by,ox[i],oy[i],ow[i],oh[i]))
            {
                oState[i] = 0;
                oFill[i] = 0;
                bState = 0;
            }
        }
        if(0!=tState && true==CheckCollision(bx,by,tx,ty,tw,th))
        {
            std::cout<< "\n\n   S.H.O.T.D.O.W.N !" <<"\n";
            break;
        }
        // Moving <<

        // Shooting >>
        if(0==bState && FSKEY_SPACE==key)
        {
            bx = 5.0;
            by = 5.0;
            bState = 1;
            vx = bvel*cos(angle*(PI/180.0));
            vy = bvel*sin(angle*(PI/180.0));
            btx = {};
            bty = {};
        }
        if(0!=bState)
        {
            vy -= G*dt;
            bx += vx*dt;
            by += vy*dt;
            btx.push_back(bx);
            bty.push_back(by);
        }
        if (bx>80 || by<-5)
        {
            bState = 0;
        }
        // Shooting <<

        // Drawing >>
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        DrawCannon(angle,cx,cy);
        for(int i=0; i<nObstacles; ++i)
        {
            DrawObstacle(10*ox[i],600 - 10*oy[i],10*ow[i],10*oh[i],oFill[i]);
        }
        DrawTarget(tx*10,600 - ty*10,tw*10,th*10);

        if(0!=bState)
        {
            DrawCannonBall(bx*10.0, 600.0 - by*10.0,brad,1);
            if(btx.size()>1)
            {
                for(int i=0; i<btx.size()-1; ++i)
                {
                    DrawTrajectory(10*btx[i],600- 10*bty[i],10*btx[i+1],600- 10*bty[i+1]);
                }
            }
        }
        glFlush();
        FsSwapBuffers();
        // Drawing <<

        FsSleep(25);
    }
    return 0;
}