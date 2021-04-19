#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include "fssimplewindow.h"
using namespace std;

const double Tol=1e-5;
const double racketW = 10, racketH = 2, blockW = 10, blockH = 2,
              ballW = 0.8, ballH = 0.8, dt = 0.025;
const int nBlocks = 16;
const double racketX = 40, racketY = 20;

double Distance(double a,double b)
{
    double d = sqrt((a-b)*(a-b));
    return d;
}
class Blocks
{
public:
    double x,y;
    int state;
    void Initialize(void);
    void Draw(void);
    bool checkCollision(double x2, double y2);
};
void Blocks::Initialize(void)
{
    // block state 0 = not hit yet
    state = 0;
}
void Blocks::Draw(void)
{

    glColor3ub(0,0,255);
    glBegin(GL_QUADS);  
    glVertex2i(10*x         ,600 - 10*y);           // Left top corner
    glVertex2i(10*x         ,600 - 10*(y-blockH));  // Left bottom corner
    glVertex2i(10*(x+blockW),600 - 10*(y-blockH));  // Right Bottom corner
    glVertex2i(10*(x+blockW),600 - 10*y);           // Right Top Corner
    glEnd();  
}
bool Blocks::checkCollision(double x2, double y2)
{
    // block state 0 = not hit, 1 = Hit
    if(0==state && Distance(x+ 0.5*blockW,x2)<(0.5*blockW + Tol) && 
            Distance(y- 0.5*blockH,y2)<(0.5*blockH + Tol))
    {
        return true;
    }
    return false;
}

class Ball
{
public:
    double x,y,vx,vy;
    int state = 0;
    void Initialize(void);
    void Draw(void);
    void Move(void);
    void Bounce(double x2, double y2,int mode);
};

void Ball::Initialize(void)
{
    state = 1;
    x =     10; // m
    y =     40; // m
    vx =    32; // m/s
    vy =   -32; // m/s
}
void Ball::Draw(void)
{
    glColor3ub(0,0,0);
    glBegin(GL_QUADS);  
    glVertex2i(10*x        ,600 - 10*y);           // Left top corner
    glVertex2i(10*x        ,600 - 10*(y-ballH));   // Left bottom corner
    glVertex2i(10*(x+ballW),600 - 10*(y-ballH));   // Right Bottom corner
    glVertex2i(10*(x+ballW),600 - 10*y);           // Right Top Corner
    glEnd();  
}
void Ball::Move(void)
{
    x+=vx*dt; // 32*0.025 = 0.8 m per loop(25ms) = 8 pixel per loop
    y+=vy*dt; // 32*0.025 = 0.8 m per loop(25ms) = 8 pixel per loop
}
void Ball::Bounce(double x2, double y2,int mode)
{   
    // BOUNCE with walls
    if(0==mode)
    {
        if(x<0 || x>80)
        {
            vx = -vx;
        }
        if(y>60)
        {
            vy = -vy;
        }
    }
    // BOUNCE with objects(blocks)
    else if(1==mode)
    {
        if(Distance(x, x2 + 0.5*blockW)<(0.5*blockW +Tol) &&
            Distance(y, y2 - 0.5*blockH)<(0.5*blockH + Tol))
        {
            vy = -vy;
            // cout << "Ball x, y = " << x << ", " << y <<endl;
            // cout << "block x, y, = " << x2 << " , " << y2 <<endl;
            // cout << "bounced ball vx, vy = " << vx << " , " <<vy <<endl;
        }
    }
    // BOUNCE with racket
    else if(2==mode)
    {
        if(Distance(x,x2)<(0.5*racketW+ Tol) && 
           Distance(y,y2)<(0.5+ Tol) && vy <0)
        {
            vy = -vy;
        }
    }
    
}

class Racket
{
public:
    double x,y;
    int state = 0;
    void Initialize(void);
    void Move(void);
    void Draw(void);
};

void Racket::Initialize(void)
{
    x = racketX;
    y = racketY;
    state = 1;
}
void Racket::Move(void)
{
    // racketX=mouseX and rackety=mouseY
    int lb,mb,rb,mx,my;
    auto mState = FsGetMouseEvent(lb,mb,rb,mx,my);
    if(FSMOUSEEVENT_MOVE==mState)
    {
        x = (double) (mx/10.0);
        y = (double) (60 - my/10.0);
    }
    if(y>10)
    {
        y = 10;
    }
    if(y<0)
    {
        y = 0;
    }

}
void Racket::Draw(void)
{
    glColor3ub(0,0,0);
    glBegin(GL_QUADS);
    glVertex2i(10*(x - racketW/2),   600 - 10*y);               // Left top corner
    glVertex2i(10*(x - racketW/2),   600 - 10*(y - racketH));   // Left Bottom
    glVertex2i(10*(x + racketW/2),   600 - 10*(y - racketH));   // Right Bottom
    glVertex2i(10*(x + racketW/2),   600 - 10*y);               // Right Top
    glEnd();
}




int main(void)
{

    Blocks blocks[nBlocks];
    Ball ball;
    Racket racket;
    int hitCount = 0;
    // Make blocks x,y positions (left top corners)
    for(int i =0; i<nBlocks/2; ++i)
    {
        // Bottom 8 blocks
        blocks[i].x = (double)(10*i);
        blocks[i].y = (double)50;
        // Top 8 blocks
        blocks[i+8].x = (double)(10*i);
        blocks[i+8].y = (double)52;
    }
    for(auto &b : blocks)
        {
            b.Initialize();
        }

    FsOpenWindow(0,0,800,600,1);
    for(;;)
    {
        FsPollDevice();
        auto key=FsInkey();
		if(FSKEY_ESC==key)
		{
			break;
		}

        if(0 == racket.state)
        {
            racket.Initialize();
        }
        if(0 == ball.state)
        {
            ball.Initialize();
        }
        // MOVING (ball and racket) >>
        if(ball.y <0)
        {
            cout << "Miss!" << "\n";
            break;
        }
    
        racket.Move();
        ball.Move();
        ball.Bounce(0,0,0); // bounce with walls

        // Bounce with blocks, erase hit blocks, and change block state to 1.
        for(auto &b: blocks)
        {
            if(true== b.checkCollision(ball.x,ball.y))
            {
                // cout << "collision check" <<endl;
                ball.Bounce(b.x,b.y,1);
                b.state =1;
                hitCount++;
                break;
            }
        }
        if(hitCount==nBlocks)
        {
            cout << "You Win!" << endl;
            break;
        }

        ball.Bounce(racket.x,racket.y,2); // bounce with racket
        
        // << MOVING (ball and racket)
        
    
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        // DRAWING >>
        
        // Blocks
        for(auto &b : blocks)
        {   
            if(0 == b.state)
            {
                b.Draw(); // Only Draw unhit blocks
            }
            
        }
        // Racket
        
        racket.Draw();
        ball.Draw();
        glFlush();
        FsSwapBuffers();

        // << DRAWING
        FsSleep(25);
    }
}