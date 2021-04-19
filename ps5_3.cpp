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
const int windowH = 100;
const double Tol=1e-6;

// Bouncing Rocket Dodge the balls

// void DrawTrajectory(double btx1, double bty1, double btx2, double bty2)
// {
//     glColor3ub(0,0,0);
//     glBegin(GL_LINES);
//     glVertex2d(btx1,bty1);
//     glVertex2d(btx2,bty2);
//     glEnd();
// }
double DotProd(double x1,double y1,double x2,double y2)
{
	return x1*x2+y1*y2;
}
void Subtract(double &dx,double &dy,double x1,double y1,double x2,double y2)
{
	dx=x2-x1;
	dy=y2-y1;
}
double Distance(double x1,double y1,double x2,double y2)
{
	double dx=x2-x1;
	double dy=y2-y1;
	return sqrt(dx*dx+dy*dy);
}
////////////////////////////////////////////////
void ProcessCollision(
	double x1,double y1,double &vx1,double &vy1,
	double x2,double y2,double &vx2,double &vy2, int mode, int &count)
{
	auto d=Distance(x1,y1,x2,y2);
	double nx,ny;
	Subtract(nx,ny,x1,y1,x2,y2);

	double rvx1=vx2-vx1;
	double rvy1=vy2-vy1;
    
	if(d<2.0 && 0.0>DotProd(nx,ny,rvx1,rvy1))
	{
		if(Tol<d)
		{
			nx/=d;
			ny/=d;
		}

		auto k1=DotProd(nx,ny,vx1,vy1);
		auto k2=DotProd(nx,ny,vx2,vy2);

        if(0 == mode)
        {
            vx1=vx1+nx*(k2-k1);
            vy1=vy1+ny*(k2-k1);
            vx2=vx2+nx*(k1-k2);
            vy2=vy2+ny*(k1-k2);
        }
        if(0 != mode)
        {
            cout << "Damage: " << count <<"\n";
            count+=1;
            vx1=vx1+nx*(k2-k1);
            vy1=vy1+ny*(k2-k1);
        }
	}
}

void DrawWall(double x1,double x2,int floor, int ceiling)
{
    glColor3ub(0,0,0);
    glBegin(GL_LINES);
    glVertex2i(x1,floor);
    glVertex2i(x1,ceiling);
    glVertex2i(x2,floor);
    glVertex2i(x2,ceiling);
    glEnd();
}

void DrawCircle(double cx,double cy,double r)
{
	glColor3ub(0,0,0);
	glBegin(GL_TRIANGLE_FAN);
	for(int i=0; i<64; ++i)
	{
		double a=(double)i*2.0*PI/64.0;
		double s=sin(a);
		double c=cos(a);
		glVertex2d(cx+c*r,cy+s*r);
	}
	glEnd();
}
void DrawRectangle(double R[4][2])
{
    // R has 4 (x,y) points
    glColor3ub(0,0,255);
    glBegin(GL_QUADS);
    glVertex2d(R[0][0],R[0][1]);
    glVertex2d(R[1][0],R[1][1]);
    glVertex2d(R[2][0],R[2][1]);
    glVertex2d(R[3][0],R[3][1]);
    glEnd();
}

void DrawTriangle(double T[3][2])
{
    // T has 3 (x,y) points.
    glColor3ub(0,0,255);
    glBegin(GL_TRIANGLES);
    glVertex2i(T[0][0],T[0][1]);
    glVertex2i(T[1][0],T[1][1]);
    glVertex2i(T[2][0],T[2][1]);
    glEnd();
}


void DrawBoost(double B[3][2],int boostState)
{
    // B has 3(x,y) points inverted
    if(0!=boostState)
    {
        glColor3ub(255,0,0);
        glBegin(GL_TRIANGLES);
        glVertex2i(B[0][0],B[0][1]);
        glVertex2i(B[1][0],B[1][1]);
        glVertex2i(B[2][0],B[2][1]);
        glEnd();
    }
}

void DrawRocket(double R[4][2], double TL[3][2],double TR[3][2],double TU[3][2], int hit)
{
    DrawRectangle(R);
    if(hit <2)
    {  
        DrawTriangle(TL);
    }
    if(hit <4)
    {
        DrawTriangle(TR);
    }
    DrawTriangle(TU);
}



int main(void)
{
    double cx = 50.0, cy = 25.0, body = 1.0;
    double wallx1 = 10, wallx2 = 90;
    double const acc = 80.0,cbreak = 50.0, accx = 200.0, bouncex = 120;
    double cvy = 0.0,cvx = 65.0;
    int boostState = 0;
    int collideRocket = 0;
    int numDamage = 0, maxDamage = 5, count = 0;
    double maxBallvy = -150;

    srand((int)time(nullptr));
    
    const int nBall = 5 + rand()%3;
    // double x[nBall],y[nBall],vx[nBall],vy[nBall];
    double *x = new double[nBall];
    double *y = new double[nBall];
    double *vx = new double[nBall];
    double *vy = new double[nBall];
    for(int i=0; i<nBall; ++i)
    {
        x[i]=(double)((wallx1+ rand()%80));
        y[i]=(double)(windowH);
        vx[i]=(double)(rand()%55-10);
        vy[i]=(double)(rand()%12-4);
    }
    
    FsOpenWindow(0,0,windowH*10,windowH*10,1);
    glShadeModel(GL_SMOOTH);
    time_t start,end;
    time(&start);
    for(;;)
    {
        double xL1 = cx - body,xL2 = cx - 2*body,
                xR1 = cx + body,xR2 = cx + 2*body,
                yU1 = cy + body, yU2 = cy + 2*body,
                yD1 = cy - body,
                yD2 = cy - 2*body,
                bxL1 = cx - 0.5*body, bxR1 = cx+ 0.5*body;
        FsPollDevice();
		auto key=FsInkey();
		if(FSKEY_ESC==key)
		{
            cout << nBall <<"\n";
			break;
		}
        double R[4][2] = {{10*xL1,windowH*10-10*yU1},{10*xR1,windowH*10-10*yU1},{10*xR1,windowH*10-10*yD1},
                            {10*xL1,windowH*10-10*yD1}};
        double TL[3][2] = {{10*xL1,windowH*10-10*cy},{10*xL2,windowH*10-10*yD1},{10*xL1,windowH*10-10*yD1}};
        double TR[3][2] = {{10*xR1,windowH*10-10*cy},{10*xR2,windowH*10-10*yD1},{10*xR1,windowH*10-10*yD1}};
        double TU[3][2] = {{10*xL1,windowH*10-10*yU1},{10*xR1,windowH*10-10*yU1},{10*cx,windowH*10-10*yU2}};
        double B[3][2] = {{10*bxL1,windowH*10-10*yD1},{10*bxR1,windowH*10-10*yD1},{10*cx,windowH*10-10*yD2}};

        boostState = 0;
        // MOVE >>
        if (0!=FsGetKeyState(FSKEY_UP))
        {
            cvy += acc*dt;
            
            boostState = 1;
        }
        if (0!=FsGetKeyState(FSKEY_DOWN))
        {
            cvy -= cbreak*dt;
            
        }
        if (0!=FsGetKeyState(FSKEY_LEFT))
        {
            cvx -= accx*dt;
            
        }
        if (0!=FsGetKeyState(FSKEY_RIGHT))
        {
            cvx += accx*dt;
            
        }
        
        // Balls moving
        for(int i=0; i<nBall; ++i)
		{
			x[i]+=vx[i]*dt;
			y[i]+=vy[i]*dt;
			vy[i]-=G*dt;
            if(vy[i]<=maxBallvy)
            {
                vy[i] = maxBallvy;
            }

			if(y[i]<=0 && vy[i]<0.0)
			{
				y[i] = (double)windowH;
            }
			if((x[i]<=wallx1 && vx[i]<0.0) || (wallx2<=x[i] && 0.0<vx[i]))
			{
				vx[i]=-vx[i];
			}
		}
        // Balls colliding each other and with rocket>> 
        for(int i=0; i<nBall; ++i)
		{
            count = 0;
            ProcessCollision(x[i],y[i],vx[i],vy[i],cx,cy,cvx,cvy,1,numDamage);
            ProcessCollision(x[i],y[i],vx[i],vy[i],xL2,yD1,cvx,cvy,1,numDamage);
            ProcessCollision(x[i],y[i],vx[i],vy[i],xR2,yD1,cvx,cvy,1,numDamage);
            
    
			for(int j=i+1; j<nBall; ++j)
			{
				ProcessCollision(x[i],y[i],vx[i],vy[i],x[j],y[j],vx[j],vy[j],0,numDamage);
			}
		}
        // <<
        
        if(numDamage > maxDamage)
        {
            time(&end);
            double elapsed = end-start;
            cout << "You have survived " << elapsed <<" seconds.\n";
            break;
        }
        if (cy <= 1.0)
        {
            cout << "CRAHSED DOWN. . .\n";
            break;
        }
        if (cy > windowH - 25) ////////////////////BORDER LINE CEILING
        {
            cvy = -0.8*cvy;
            // if(cvy>30 || cvy <-30)
            // {
            //     cvy = cvy/2;
            // }
        }
        if (cx< wallx1)
        {
            cvx = -cvx;
            // cx = wallx1+1;
        }
        if (cx > wallx2)
        {
            cvx = -cvx;
            // cx = wallx2-;
        }
        cvy -= G*dt;
        cy += cvy*dt;
        cx += cvx*dt;
        if (cvy>160 || cvy < -160)
        {
            cvy = cvy/2;
        }
        // MOVE <<
        
        // DRAWING >>
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        
        // ROAD >>
        glBegin(GL_QUADS);
		glColor3ub(255,230,10);
		glVertex2i(10*wallx1,0);
		glColor3ub(255,55,255);
		glVertex2i(10*wallx1,10*windowH);
		glColor3ub(0,0,0);
		glVertex2i(10*wallx2,10*windowH);
		glColor3ub(20,200,20);
		glVertex2i(10*wallx2,0);
		glEnd();
        // << ROAD

        // CEILING >>
        glBegin(GL_QUADS);
        glColor3ub(255,100,100);
        glVertex2i(10*0, windowH*10 - 10*(windowH-25));
        glVertex2i(10*windowH, windowH*10 - 10*(windowH-25));
        glVertex2i(10*windowH, windowH*10 - 10*(windowH-22));
        glVertex2i(10*0, windowH*10 - 10*(windowH-22));
        glEnd();
        // << CEILING

        DrawRocket(R,TL,TR,TU,numDamage);
        DrawBoost(B,boostState);
        DrawWall(10*wallx1,10*wallx2,windowH*10,0);
        for(int i=0; i<nBall; ++i)
		{
			DrawCircle(x[i]*10.0,windowH*10.0-y[i]*10.0,12.0);
		}

        FsSwapBuffers();
        glFlush();
        // DRAWING <<
        FsSleep(25);
    }
    delete[] x;
    delete[] y;
    delete[] vx;
    delete[] vy;
    return 0;
}