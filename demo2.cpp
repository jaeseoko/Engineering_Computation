#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <math.h>
#include <vector>
#include <string>
#include <chrono>
#include "fssimplewindow.h"
#include "yssimplesound.h"
#include "ysglfontdata.h"

using namespace std;
const int windowH = 800, windowW = 1000;
const int myWindowH = 80, myWindowW = 100;
const double dt = 0.025;
const double YS_PI=3.1415927;
const double Tol =1e-6;


double oneDist(double a, double b)
{
    return sqrt((a-b)*(a-b));
}

double Distance(vector<double> a,vector<double> b)
{
    double d = sqrt((a[0]-b[0])*(a[0]-b[0]) + (a[1]-b[1])*(a[1]-b[1]));
    return d;
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
class Map
{
protected:
    
    double lineLen = 5.0, lineW = 0.5, lineInterval = 6;
    
    
    
    vector<vector<int>> RoadVert {};

public:
    Map();
    ~Map();
    const int roadW = 10;
    double centerX,centerY,turnD;
    double vertAx,vertAy,vertBx,vertBy,vertCx,vertCy,vertDx,vertDy;
    vector<double> U1,U2,L1,L2,D1,D2,R1,R2;
    vector<vector<double>> line1Vert {};
    vector<vector<double>> line2Vert {};
    vector<vector<double>> line3Vert {};
    vector<double> center {};
    void DrawBackGround() const;
    void DrawRoads() const;
    void DrawLines(double x, double y, int roadNum) const;
    void DrawSigns(double x, double y) const;
    void DispVel(double x, double y, double vel) const;
    void DispWarning(double x,double y) const;
};
Map::Map()
{
    turnD = roadW/2;
    centerX = 50,centerY=40;
    center = {centerX,centerY};
    vertAx = centerX - roadW/4,vertAy = centerY + roadW/4;
    vertBx = centerX - roadW/4,vertBy = centerY - roadW/4;
    vertCx = centerX + roadW/4,vertCy = centerY - roadW/4;
    vertDx = centerX + roadW/4,vertDy = centerY + roadW/4; 
    U1 = {45,80},U2 = {55,80},D1 = {45,0}, D2 = {55,0},
    L1 = {0,45}, L2 = {0,35} ,R1 = {100,35},R2 = {100,45};
    
    for(int i=0; i<3; ++i)
    {
        line1Vert.push_back({centerX-lineW/2,75 - i*(lineLen + lineInterval)});
    }
    for(int i=0; i<3; ++i)
    {
        line1Vert.push_back({centerX-lineW/2,30 - i*(lineLen + lineInterval)});
    }
    for(int i=0; i<3; ++i)
    {
        line2Vert.push_back({8+i*(lineLen + lineInterval+2),centerY});
    }
    for(int i =0; i<3; ++i)
    {
        line2Vert.push_back({63+i*(lineLen + lineInterval+2),centerY});
    }
    
                 
}
Map::~Map()
{

}
void Map::DrawBackGround() const
{   
    glColor3ub(23,141,19);
    glBegin(GL_QUADS);
    glVertex2i(0,0);
    glVertex2i(0,800);
    glVertex2i(1000,800);
    glVertex2i(1000,0);
    glEnd();
}
void Map::DrawRoads() const
{
    
    glBegin(GL_QUADS);
    glColor3ub(75,75,75);
    // Road 1
    glVertex2i(10*U1[0],windowH - 10*U1[1]),glVertex2i(10*D1[0],windowH - 10*D1[1]);
    glVertex2i(10*D2[0],windowH - 10*D2[1]),glVertex2i(10*U2[0],windowH - 10*U2[1]);
    // Road 2
    glVertex2i(10*L1[0],windowH - 10*L1[1]),glVertex2i(10*L2[0],windowH - 10*L2[1]);
    glVertex2i(10*R1[0],windowH - 10*R1[1]),glVertex2i(10*R2[0],windowH - 10*R2[1]);
    
    glEnd();
    glColor3ub(255,255,255);
    DrawCircle(10*centerX,windowH - 10*centerY,5,1);
    
}
void Map::DrawLines(double x, double y,int roadNum) const
{
    
    //double lineLen = 5.0, lineW = 1.0, lineInterval = 3.5;
    glColor3ub(255,255,255);
    glBegin(GL_QUADS);
    // Line For Road 1 (VERTICAL) 2lines x 3. Total 6 lines
    
    if(1==roadNum)
    {
        glVertex2d(10*(x),windowH - 10*y);
        glVertex2d(10*(x),windowH - 10*(y - lineLen));
        glVertex2d(10*(x+lineW),windowH - 10*(y - lineLen));
        glVertex2d(10*(x+lineW),windowH - 10*y);
    }

    
    // Line For Road 2 (HORIZONTAL)
    else
    {
        glVertex2d(10*(x),windowH - 10*y);
        glVertex2d(10*(x),windowH - 10*(y - lineW));
        glVertex2d(10*(x+lineLen),windowH - 10*(y - lineW));
        glVertex2d(10*(x+lineLen),windowH - 10*y);
    }
    glEnd();
}
void Map::DrawSigns(double x, double y) const
{
    int W = 12, H = 7, L = 4;
    glColor3ub(255,255,255);
    glBegin(GL_QUADS);
    glVertex2i(10*x,windowH - 10*y);
    glVertex2i(10*x,windowH - 10*(y - H));
    glVertex2i(10*(x+W),windowH - 10*(y - H));
    glVertex2i(10*(x+W),windowH - 10*y);
    glEnd();
    glColor3ub(255,255,255);
    glBegin(GL_LINES);
    glVertex2i(10*(x+W/2),windowH - 10*(y - H));
    glVertex2i(10*(x+W/2),windowH - 10*(y - H - L));
    glEnd();
    
}
void Map::DispVel(double x, double y, double vel) const
{
    glColor3ub(0,0,0);
    glRasterPos2i(10*(x-1),windowH - 10*y);
    YsGlDrawFontBitmap10x14("YOUR SPEED:");
    glRasterPos2d(10*(x+3.5),windowH - 10*y +35);
    char str[256];
	sprintf(str,"%d",(int)vel);
    YsGlDrawFontBitmap10x14(str);
}
void Map::DispWarning(double x,double y) const
{
    glColor3ub(0,0,0);
    glRasterPos2d(10*(x-0.2),windowH - 10*(y-1));
    YsGlDrawFontBitmap10x14("D R I V E");
    glColor3ub(0,0,0);
    glRasterPos2d(10*(x+0.5),windowH - 10*(y-1) +25);
    YsGlDrawFontBitmap10x14("S A F E");
}

class Car
{
protected:
    double len = 3,width = 1.5, wheelR=0.5;
public:
    void Initialize(int carN,int laneNum);
    Map map;
    // ~Car();
    vector<vector<double>> startPoints {}; // ind 0: road1 right lane, 1:road1 left lane...
    int myLane;
    int laneNum, laneDir,carNum;
    int turnCount=0,exitCount = 0;
    int turnMode = 0;   // At Intersection 0: STRAIGHT, 1: RIGHT, 2: LEFT
    double x,y;         // THIS IS CAR'S CENTER
    const double maxVel = 12,minVel = 0;
    double vel,acc,brkacc;
    int state = 0;      //0: horizontal 1: vertical    
    double breakingD;
    void Draw() const;
    void Move();
    void Turn(int turning);
    void Stop();
};
void Car::Initialize(int carN,int lane)
{
    startPoints = {{map.centerX-map.roadW/4,80},            // U1 going dowon
                   {map.centerX+map.roadW/4,0},             // D2 going Up
                   {0,map.centerY-map.roadW/4},             // L2 going Right
                   {100,map.centerY+map.roadW/4}};          // R2 going Left
    
    laneNum = lane;
    carNum = carN;
    acc = 3.4 + 0.5*(double)carNum;
    brkacc = acc*2;
    breakingD = 2*brkacc;
    if(1==laneNum)
    {
        state = 1;
        x=map.vertAx, y=80;
        myLane = 1;
    }
    else if(2==laneNum)
    {
        state = 1;
        x=map.vertDx, y=0;
        myLane = 2;
    }
    else if(3==laneNum)
    {
        state = 0;
        x=0, y=map.vertBy;
        myLane = 3;
    }
    else if(4==laneNum)
    {
        state = 0;
        x=100,y=map.vertDy;
        myLane = 4;
    }
    
    
}

void Car::Draw() const
{
    if(carNum%4==0) glColor3ub(255,0,0);
    else if(carNum%4==1) glColor3ub(255,255,0);
    else if(carNum%4==2) glColor3ub(0,120,255);
    else if(carNum%4==3) glColor3ub(255,255,255);
    
    if(state==0)                                            // Car horizontal (on lane 3,4)
    {
        glBegin(GL_QUADS);
        glVertex2d(10*(x-len/2),windowH - 10*(y+width/2));
        glVertex2d(10*(x-len/2),windowH - 10*(y-width/2));
        glVertex2d(10*(x+len/2),windowH - 10*(y-width/2));
        glVertex2d(10*(x+len/2),windowH - 10*(y+width/2));
    }
    else if(state==1)                                      // Car Vertical (on lane 1,2)
    {
          
        glBegin(GL_QUADS);
        glVertex2d(10*(x-width/2),windowH - 10*(y+len/2));
        glVertex2d(10*(x-width/2),windowH - 10*(y-len/2));
        glVertex2d(10*(x+width/2),windowH - 10*(y-len/2));
        glVertex2d(10*(x+width/2),windowH - 10*(y+len/2));
    }
    
    glEnd();
}
void Car::Move()
{
    
    if(y<0-len/2 || y>len/2 +myWindowH || x<0 -width/2|| x>myWindowW+width/2)
    {
        if(1==laneNum)      laneNum+=1,x = map.vertDx,y=0;
        else if(2==laneNum) laneNum+=1,x = 0         ,y=map.vertBy;
        else if(3==laneNum) laneNum+=1,x = 100       ,y=map.vertDy;
        else if(4==laneNum) laneNum=1 ,x = map.vertAx,y=80;
        exitCount++;
        turnCount=exitCount;
        
    }
    vel+=acc*dt;
    if(vel>maxVel)      vel=maxVel;
    if(vel<minVel)      vel=minVel;
    if(1==laneNum)      state = 1, y-=vel*dt,x=map.vertAx;
    else if(2==laneNum) state = 1, y+=vel*dt, x = map.vertCx;
    else if(3==laneNum) state = 0, x+=vel*dt,y=map.vertBy;
    else if(4==laneNum) state = 0, x-=vel*dt, y=map.vertAy;
    
    
    
    
}
void Car::Turn(int turning)
{
                                    // 1 = RIGHT TURN, 2 = LEFT TURN
    if(turnCount == exitCount)
    {
        if(1==laneNum)
        {
            
            if(1==turning && Distance({x,y},{map.vertAx,map.vertAy})<0.3) laneNum = 4,turnCount++;
            else if(2==turning && Distance({x,y},{map.vertBx,map.vertBy})<0.3) laneNum = 3,turnCount++;
            
        }
        else if(2==laneNum)
        {
            if(1==turning&& Distance({x,y},{map.vertCx,map.vertCy})<0.3) laneNum = 3,turnCount++;
            else if(2==turning && Distance({x,y},{map.vertDx,map.vertDy})<0.3) laneNum = 4,turnCount++;
            
        }
        else if(3==laneNum)
        {
            if(1==turning && Distance({x,y},{map.vertBx,map.vertBy})<0.3) laneNum = 1,turnCount++;
            else if(2==turning && Distance({x,y},{map.vertCx,map.vertCy})<0.3) laneNum = 2,turnCount++;   
            
        }
        else if(4==laneNum)
        {
            if(1==turning && Distance({x,y},{map.vertDx,map.vertDy})<0.3) laneNum = 2,turnCount++;
            else if(2==turning && Distance({x,y},{map.vertAx,map.vertAy})<0.3) laneNum = 1,turnCount++;
            
        }
    }
    else
    {
    }
}
void Car::Stop()
{
    vel-=brkacc*dt;
}
class Signals
{
public:
    Map map;
    void Initialize(int mode);
    int x,y;
    bool go;
    void Draw(int mode) const;
};
void Signals::Initialize(int mode)
{
    go = true;
    if(1==mode)
    {
        x = map.U1[0]-7,y = map.L1[1]+12;
    }
    else if(2==mode)
    {
        x = map.U2[0]+7,y = map.R1[1]-2;
    }    
    else if(3==mode)
    {
        x = map.U1[0]-12,y = map.L2[1]-2;    
    }
    else
    {
        x = map.U2[0]+2,y = map.R2[1]+7;    
    }
}

void Signals::Draw(int mode) const
{
    int W= 4, L = 9, rad = 2, gap = 5;
    if(mode<3)
    {
        glColor3ub(0,0,0);
        glBegin(GL_QUADS);
        glVertex2i(10*x,windowH - 10*y);
        glVertex2i(10*x,windowH - 10*(y-L));
        glVertex2i(10*(x+W),windowH - 10*(y-L));
        glVertex2i(10*(x+W),windowH - 10*y);
        glEnd();
        if(go==true)
        {
            glColor3ub(0,150,0);
            DrawCircle(10*(x+rad),windowH-10*(y-rad),10*rad,1);
        }
        else
        {
            glColor3ub(200,0,0);
            DrawCircle(10*(x+rad),windowH-10*(y-rad - gap),10*rad,1);
        }
        glEnd();
    }
    if(mode>3)
    {
        glColor3ub(0,0,0);
        glBegin(GL_QUADS);
        glVertex2i(10*x,windowH - 10*y);
        glVertex2i(10*x,windowH - 10*(y-W));
        glVertex2i(10*(x+L),windowH - 10*(y-W));
        glVertex2i(10*(x+L),windowH - 10*y);
        glEnd();
        if(go==true)
        {
            glColor3ub(0,150,0);
            DrawCircle(10*(x+rad),windowH-10*(y-rad),10*rad,1);
        }
        else
        {
            glColor3ub(200,0,0);
            DrawCircle(10*(x+rad+gap),windowH-10*(y-rad),10*rad,1);
        }
        glEnd();
    }
}



int main(void)
{
    double speed = 0;

    YsSoundPlayer player;
    YsSoundPlayer::SoundData TrafficSE;

    FsChangeToProgramDir();
	if(YSOK!=TrafficSE.LoadWav("Traffic.wav"))
	{
		printf("Load error.\n");
		return 1;
	}

    Map map;
    Signals sig1,sig2,sig3,sig4;
    sig1.Initialize(1),sig2.Initialize(1),sig3.Initialize(1),sig4.Initialize(1);
    
    Car car1,car2,car3,car4;
    car1.Initialize(1,1);
    car2.Initialize(2,2);
    car3.Initialize(3,3);
    car4.Initialize(4,4);
    vector<Car> cars {car1,car2,car3,car4};
    // srand(time(NULL));
    
    time_t start = time(0),breaking,breakingEnd;
    FsOpenWindow(16,16,windowW,windowH,1);
    player.Start();
    for(;;)
    {
        player.KeepPlaying();
        player.PlayBackground(TrafficSE);

        FsPollDevice();
        auto key=FsInkey();
        int tPassed = difftime(time(0), start);
        if(FSKEY_ESC==key)
        {
            break;
        }
        if(tPassed>60)
        {
            break;
        }

        // vector<int> decide = {tPassed%3,(tPassed+rand())%3,)}
        if(tPassed>5 && cars.size()==4)
        {
            cout << "check" << endl;
            Car car5;
            car5.Initialize(5,1);
            cars.push_back(car5);
        }
        
        // car1.Turn(2);
        // car2.Turn(1);
        
        for(auto &c : cars)
        {
            c.Turn(1);
            c.Move();
            if(c.x > map.vertDx && c.y-map.vertDy < Tol)
            {
                speed = c.vel;
            }
            else
            {
                speed = 0;
            }
            
        }
        // BREAKING TEST
        if(car1.vel > 8)
        {
            car1.Stop();
        }
        

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 
        map.DrawBackGround();
        map.DrawRoads();
        map.DrawSigns(map.U2[0]+10,map.R2[1]+15);
        map.DispVel(map.U2[0]+10+2,map.R2[1]+15 - 2,speed);
        map.DrawSigns(map.D1[0]-25,map.L2[1]-5);
        map.DispWarning(map.D1[0]-25 + 2,map.L2[1]-5 - 2);
        sig1.Draw(1),sig2.Draw(2),sig3.Draw(3),sig4.Draw(4);
        for(auto &l1 : map.line1Vert)
        {
            map.DrawLines(l1[0],l1[1],1);
        }
        for(auto &l2 : map.line2Vert)
        {
            map.DrawLines(l2[0],l2[1],2);
        }
        for(auto &c: cars)
        {
            c.Draw();
        }
        FsSwapBuffers();
        FsSleep(25);
    }
}   
