#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "fssimplewindow.h"

const int nObstacle=5;

double Distance(double x1,double x2)
{
    return sqrt((x1-x2)*(x1-x2));
}
void DrawLine(int x1,int y1,int x2,int y2)
{
    glBegin(GL_LINES);

    glVertex2i(x1,y1);
    glVertex2i(x2,y2);

    glEnd();
}

void DrawRect(int x1,int y1,int x2,int y2,int fill)
{
    if(0!=fill)
    {
        glBegin(GL_QUADS);
    }
    else
    {
        glBegin(GL_LINE_LOOP);
    }

    glVertex2i(x1,y1);
    glVertex2i(x2,y1);
    glVertex2i(x2,y2);
    glVertex2i(x1,y2);

    glEnd();
}
int CheckHitTarget(
    double missileX,double missileY,
    double targetX,double targetY,double targetSizeX,double targetSizeY)
{
    double relativeX,relativeY;
    relativeX=Distance(missileX,targetX+0.5*targetSizeX);
    relativeY=Distance(missileY,targetY+0.5*targetSizeY);
    if(0<=relativeX && relativeX<(0.5*targetSizeX) && 
        0<=relativeY && relativeY<(0.5*targetSizeY))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
void PhysicalCoordToScreenCoord(int &sx,int &sy,double px,double py)
{
	sx=(int)(px*10.0);
	sy=600-(int)(py*10.0);
}
void DrawCircle(int cx,int cy,int rad,int fill)
{
    const double YS_PI=3.1415927;

    if(0!=fill)
    {
        glBegin(GL_POLYGON);
    }
    else
    {
        glBegin(GL_LINE_LOOP);
    }

    int i;
    for(i=0; i<64; i++)
    {
        double angle=(double)i*YS_PI/32.0;
        double x=(double)cx+cos(angle)*(double)rad;
        double y=(double)cy+sin(angle)*(double)rad;
        glVertex2d(x,y);
    }

    glEnd();
}
class CannonBall
{
public:
    int state;
    double x,y,vx,vy;
    void Draw(void);
};

void CannonBall::Draw(void)
{
    int sx, sy;
	PhysicalCoordToScreenCoord(sx,sy,x,y);

    glColor3ub(255,0,0);
    DrawCircle(sx,sy,5,1);
}

class Target
{
public:
    double x,y,w,h;
    void Draw(void);
    int IsHitByBall(CannonBall &ball);
};

void Target::Draw(void)
{
	int x1,y1,x2,y2;
	PhysicalCoordToScreenCoord(x1,y1,x,y);
	PhysicalCoordToScreenCoord(x2,y2,x+w,y+h);

    glColor3ub(255,0,0);
    DrawRect(x1,y1,x2,y2,1);
}

int Target::IsHitByBall(CannonBall &ball)
{
    return CheckHitTarget(ball.x,ball.y,x,y,w,h);
}

class Obstacle
{
public:
    int state;
    double x,y,w,h;
    void Draw(void);
    int IsHitByBall(CannonBall &ball);
};

void Obstacle::Draw(void)
{
    // DrawObstacle(nObstacle, obstacle)   
    int x1,y1,x2,y2;
    PhysicalCoordToScreenCoord(x1,y1,x,y);
    PhysicalCoordToScreenCoord(x2,y2,x+w,y+h);
    glColor3ub(0,255,0);
    DrawRect(x1,y1,x2,y2,state);
}

int Obstacle::IsHitByBall(CannonBall &ball)
{
   return CheckHitTarget(ball.x,ball.y,x,y,w,h);
}

void DrawArtillery(double x,double y,double angle)
{
	int sx,sy;
	PhysicalCoordToScreenCoord(sx,sy,x,y);

    glColor3ub(0,0,255);
    DrawRect(sx-5,sy-5,sx+5,sy+5,1);

	int vx,vy;
	PhysicalCoordToScreenCoord(vx,vy,x+3.0*cos(angle),y+3.0*sin(angle));

    DrawLine(sx,sy,vx,vy);
}

void MoveCannon(double &x,double &y,double &vx,double &vy,double m,double dt)
{
    x+=vx*dt;
    y+=vy*dt;

    vy-=9.8*dt;
}

void FireCannon(
    double &cx,double &cy,double &vx,double &vy,
    double artX,double artY,double artAngle,double iniVel)
{
    cx=artX;
    cy=artY;
    vx=iniVel*cos(artAngle);
    vy=iniVel*sin(artAngle);
}

void GenerateObstacle(int nObstacle, Obstacle obstacle[])
{
    int i;
    for(i=0; i<nObstacle; i++)
    {
        obstacle[i].state=1;
        obstacle[i].x=(double)(10+rand()%70);
        obstacle[i].y=(double)(rand()%60);
        obstacle[i].w=(double)(8+rand()%8);
        obstacle[i].h=(double)(8+rand()%8);

        // The following if-statements forces the effective size of the
        // obstacle to be between 8x8 and 15x15.
        if(80.0<obstacle[i].x+obstacle[i].w)
        {
            obstacle[i].x=80.0-obstacle[i].w;
        }
        if(60.0<obstacle[i].y+obstacle[i].h)
        {
            obstacle[i].y=60.0-obstacle[i].h;
        }
    }
}

void DrawObstacle(int nObstacle,Obstacle obstacle[])
{
    for(int i=0; i<nObstacle; i++)
    {
		obstacle[i].Draw();
    }
}

void MoveTarget(double &targetX,double &targetY,const double dt)
{
	targetY-=10*dt;
	if(0>targetY)
	{
		targetY=60.0;
	}
}

// For extra credit >>
void DrawTrajectory(int nPnt,const double pnt[])
{
	if(2<=nPnt)
	{
		glColor3ub(0,0,0);
		glBegin(GL_LINE_STRIP);

		int i;
		for(i=0; i<nPnt; i++)
		{
			int sx,sy;
			PhysicalCoordToScreenCoord(sx,sy,pnt[i*2],pnt[i*2+1]);
			glVertex2i(sx,sy);
		}

		glEnd();
	}
}
// For extra credit <<



int main(void)
{
    int i,key,nShot;
    double artX,artY,artAngle;
    const double PI=3.1415927;

	// For extra credit >>
	const int maxNumTrj=200;
	int nTrj=0;
	double trj[maxNumTrj*2];
	// For extra credit <<

    artX=5.0;
    artY=5.0;
    artAngle=PI/6.0;

    srand(time(NULL));

    CannonBall cannonBall;
    cannonBall.state=0;

    Target target;
    target.x=75.0;
    target.y=60.0;
    target.w=5.0;
    target.h=5.0;

    FsOpenWindow(16,16,800,600,1);

    Obstacle obstacle[nObstacle];
    GenerateObstacle(nObstacle,obstacle);


    FsPollDevice();

    nShot=0;
    while(FSKEY_ESC!=(key=FsInkey()))
    {
        FsPollDevice();

        switch(key)
        {
        case FSKEY_SPACE:
            if(cannonBall.state==0)
            {
                cannonBall.state=1;
                FireCannon(
                    cannonBall.x,cannonBall.y,cannonBall.vx,cannonBall.vy,
                    artX,artY,artAngle,40.0);
                nShot++;

				// For extra credit >>
				nTrj=0;
				// For extra credit <<
            }
            break;
        }

		artAngle+=PI/180.0;
		if(PI/2.0<artAngle)
		{
			artAngle=0.0;
		}

		MoveTarget(target.x,target.y,0.025);

        if(cannonBall.state==1)
        {
            MoveCannon(cannonBall.x,cannonBall.y,cannonBall.vx,cannonBall.vy,1.0,0.02);
            if(cannonBall.y<0.0 || cannonBall.x<0.0 || 80.0<cannonBall.x)
            {
                cannonBall.state=0;
            }

			// For extra credit >>
			if(nTrj<maxNumTrj)
			{
				trj[nTrj*2  ]=cannonBall.x;
				trj[nTrj*2+1]=cannonBall.y;
				nTrj++;
			}
			// For extra credit <<

            for(i=0; i<nObstacle; i++)
            {
                if(obstacle[i].state==1 && 
                   obstacle[i].IsHitByBall(cannonBall)==1)
                {
                    printf("Hit Obstacle!\n");
                    obstacle[i].state=0;
                    cannonBall.state=0;
                }
            }

            if(target.IsHitByBall(cannonBall)==1)
            {
                printf("Hit Target!\n");
                printf("You fired %d shots.\n",nShot);
                break;
            }
        }

        glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
        DrawArtillery(artX,artY,artAngle);
        DrawObstacle(nObstacle,obstacle);
        target.Draw();
        if(cannonBall.state==1)
        {
            cannonBall.Draw();
			// For extra credit >>
			DrawTrajectory(nTrj,trj);
			// For extra credit <<
        }
        FsSwapBuffers();

        FsSleep(25);
    }

    return 0;
}
