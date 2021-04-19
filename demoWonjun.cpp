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

vector<double> matmul(vector<vector<double>> R, vector<double> a)
{
    double x = R[0][0]*a[0] + R[0][1]*a[1];
    double y = R[1][0]*a[0] + R[1][1]*a[1];
    return vector<double> {x,y};
}
vector<double> RotatePoint(vector<double> RotateAbout,vector<double> RotateThis,double angle)
{
    vector<vector<double>> Rotation = {{cos(angle),-sin(angle)},
                                        {sin(angle),cos(angle)}};
    vector<double> Rotated = matmul(Rotation,{RotateThis[0]-RotateAbout[0],RotateThis[1]-RotateAbout[1]});
    Rotated[0] += RotateAbout[0];
    Rotated[1] += RotateAbout[1];
    return Rotated;
}
double oneDist(double a, double b)
{
    return sqrt((a-b)*(a-b));
}

double Distance(vector<double> a,vector<double> b)
{
    double d = sqrt((a[0]-b[0])*(a[0]-b[0]) + (a[1]-b[1])*(a[1]-b[1]));
    return d;
}

void DrawCircle(double cx,double cy,double rad,int fill)
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
    double angle = YS_PI;
    
    void DrawBackGround() const;
    void DrawRoads() const;
    void DrawLines(double x, double y, int roadNum) const;
    void DrawSigns(double x, double y) const;
    void DispVel(double x, double y, double vel) const;
    void DispWarning(double x,double y) const;
    void DrawTree(double x, double y) const;
    void DrawWindMill(double x, double y);
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
    glRasterPos2d(10*(x+1),windowH - 10*y +35);
    char str[256];
	sprintf(str,"%d",(int)vel);
    YsGlDrawFontBitmap10x14(str);
    glRasterPos2d(10*(x+4),windowH - 10*y +35);
    YsGlDrawFontBitmap10x14("MPH");
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
void Map::DrawTree(double x, double y) const
{
    glColor3ub(139,69,19);
    glBegin(GL_QUADS);
    glVertex2d(10*(x - 0.5),windowH - 10*y);
    glVertex2d(10*(x + 0.5),windowH - 10*y);
    glVertex2d(10*(x + 0.5),windowH - 10*(y+3));
    glVertex2d(10*(x - 0.5),windowH - 10*(y+3));
    glEnd();
    glColor3ub(0,230,0);
    glBegin(GL_TRIANGLES);
    glVertex2d(10*x,windowH - 10*(y+3+3));
    glVertex2d(10*(x-1.5),windowH - 10*(y+3));
    glVertex2d(10*(x+1.5),windowH - 10*(y+3));
    glColor3ub(0,230,0);
    glBegin(GL_TRIANGLES);
    glVertex2d(10*x,windowH - 10*(y+3+3+1.5));
    glVertex2d(10*(x-1.5),windowH - 10*(y+3+1.5));
    glVertex2d(10*(x+1.5),windowH - 10*(y+3+1.5));
    glColor3ub(0,230,0);
    glBegin(GL_TRIANGLES);
    glVertex2d(10*x,windowH - 10*(y+3+3+3));
    glVertex2d(10*(x-1.5),windowH - 10*(y+3+3));
    glVertex2d(10*(x+1.5),windowH - 10*(y+3+3));
    glEnd();
}
void Map::DrawWindMill(double x, double y)
{
    double rad = 5;
    double x1 = x+0.5,x2 = x-0.5;
    double y1 = y+rad;
    // vector<vector<double>> Rotation1 = {{cos(angle),-sin(angle)},
    //                                     {sin(angle),cos(angle)}};
    // vector<vector<double>> Rotation2 = {{cos(angle+2/3*YS_PI),-sin(angle+2/3*YS_PI)},
    //                                     {sin(angle+2/3*YS_PI),cos(angle+2/3*YS_PI)}}; 
    // vector<vector<double>> Rotation3 = {{cos(angle+4/3*YS_PI),-sin(angle+4/3*YS_PI)},
    //                                     {sin(angle+4/3*YS_PI),cos(angle+4/3*YS_PI)}};                                                                       
                                        
    vector<double> point1 = RotatePoint({x,y},{x1,y1},angle);
    vector<double> point2 = RotatePoint({x,y},{x2,y1},angle);
    vector<double> point3 = RotatePoint({x,y},{x1,y1},angle+2/3*YS_PI);
    vector<double> point4 = RotatePoint({x,y},{x2,y1},angle+2/3*YS_PI);
    vector<double> point5 = RotatePoint({x,y},{x1,y1},angle+4/3*YS_PI);
    vector<double> point6 = RotatePoint({x,y},{x2,y1},angle+4/3*YS_PI);


    
    glColor3ub(200,200,200);
    glBegin(GL_QUADS);
    glVertex2d(10*(x-0.5),windowH-10*y);
    glVertex2d(10*(x-1),windowH-10*(y-5));
    glVertex2d(10*(x+1),windowH-10*(y-5));
    glVertex2d(10*(x+0.5),windowH-10*y);
    glEnd();
    glColor3ub(255,255,255);
    DrawCircle(10*x,windowH-10*y,5,1);
    glEnd();
    glColor3ub(255,255,255);
    glBegin(GL_TRIANGLES);
    glVertex2d(10*x,windowH-10*y);
    glVertex2d(10*point1[0],windowH-10*point1[1]);
    glVertex2d(10*point2[0],windowH-10*point2[1]);

    glVertex2d(10*x,windowH-10*y);
    glVertex2d(10*point3[0],windowH-10*point3[1]);
    glVertex2d(10*point4[0],windowH-10*point4[1]);

    glVertex2d(10*x,windowH-10*y);
    glVertex2d(10*point5[0],windowH-10*point5[1]);
    glVertex2d(10*point6[0],windowH-10*point6[1]);
    glEnd();
    

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
    bool special_car;
    int turnCount=0,exitCount = 0;
    int turnMode = 0;   // At Intersection 0: STRAIGHT, 1: RIGHT, 2: LEFT
    double x,y;         // THIS IS CAR'S CENTER
    const double maxVel = 18,minVel = 0;
    double vel,acc,brkacc;
    double dist;
    int state = 0;      //0: horizontal 1: vertical    
    double breakingD;
    void Draw() const;
    void Move();
    void Turn(int turning);
    void Stop();
    bool check_near(const Car* other_car)
    {
        if(pow(x-other_car->x,2)+pow(y-other_car->y,2)<15 && laneNum == other_car->laneNum)
        
        {
            
            vel = 0;

            return true;
        }
        // vel += acc*dt;
        return false;
    }
    bool check_collision(const vector<Car*>& cars_pointers){
        for(int i=0;i<cars_pointers.size();++i){
            if(this == cars_pointers[i]) return false;
            if(check_near(cars_pointers[i]))
            {
                return true;
            }
        }
        return false;
    }
};
void Car::Initialize(int carN,int lane)
{
    dist = Distance({x,y},map.center);
    startPoints = {{map.centerX-map.roadW/4,80},            // U1 going dowon
                   {map.centerX+map.roadW/4,0},             // D2 going Up
                   {0,map.centerY-map.roadW/4},             // L2 going Right
                   {100,map.centerY+map.roadW/4}};          // R2 going Left
    
    laneNum = lane;
    carNum = carN;
    acc = 3.0 + 0.2*(double)carNum;
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

    if(rand()%30 ==0){
        special_car = true;
    }
    else
    {
        special_car = false;
    }
    
}

void Car::Draw() const
{

    if(state==0)                                            // Car horizontal (on lane 3,4)
    {
        
        glColor3ub(0,0,0);
        DrawCircle(10*(x-width/2),windowH - 10*(y+width/2),10*0.5,1);
        DrawCircle(10*(x+width/2),windowH - 10*(y+width/2),10*0.5,1);
        DrawCircle(10*(x-width/2),windowH - 10*(y-width/2),10*0.5,1);
        DrawCircle(10*(x+width/2),windowH - 10*(y-width/2),10*0.5,1);
        glEnd();
    }
    else if(state==1)                                      // Car Vertical (on lane 1,2)
    {
        glColor3ub(0,0,0);
        DrawCircle(10*(x-width/2),windowH - 10*(y+width/2),10*0.5,1);
        DrawCircle(10*(x+width/2),windowH - 10*(y+width/2),10*0.5,1);
        DrawCircle(10*(x-width/2),windowH - 10*(y-width/2),10*0.5,1);
        DrawCircle(10*(x+width/2),windowH - 10*(y-width/2),10*0.5,1);
        glEnd();
    }

    if(special_car == false){
        if(carNum%4==0) glColor3ub(255,0,0);
        else if(carNum%4==1) glColor3ub(255,255,0);
        else if(carNum%4==2) glColor3ub(0,120,255);
        else if(carNum%4==3) glColor3ub(255,255,255);
    }else{
        glColor3ub(rand()%255,rand()%255,rand()%255);
    }
    
    
    if(state==0)                                            // Car horizontal (on lane 3,4)
    {
        glBegin(GL_QUADS);
        glVertex2d(10*(x-len/2),windowH - 10*(y+width/2));
        glVertex2d(10*(x-len/2),windowH - 10*(y-width/2));
        glVertex2d(10*(x+len/2),windowH - 10*(y-width/2));
        glVertex2d(10*(x+len/2),windowH - 10*(y+width/2));
        glEnd();
        
        
        
    }
    else if(state==1)                                      // Car Vertical (on lane 1,2)
    {
        glBegin(GL_QUADS);
        glVertex2d(10*(x-width/2),windowH - 10*(y+len/2));
        glVertex2d(10*(x-width/2),windowH - 10*(y-len/2));
        glVertex2d(10*(x+width/2),windowH - 10*(y-len/2));
        glVertex2d(10*(x+width/2),windowH - 10*(y+len/2));
        glEnd();
           
    }
}
void Car::Move()
{
    
    if(y<0-len/2 || y>len/2 +myWindowH || x<0 -width/2|| x>myWindowW+width/2)
    {
        if(1==laneNum)      laneNum+=1,x = map.vertDx,y=0;
        else if(2==laneNum) laneNum+=1,x = 0      ,y=map.vertBy;
        else if(3==laneNum) laneNum+=1,x = 100     ,y=map.vertDy;
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
                                    // 0 = STRAIGHT , 1 = RIGHT TURN, 2 = LEFT TURN
    
    if(0==turning)
    {
    }
    else if(turnCount == exitCount)
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
}
void Car::Stop()
{
    vel = 0;
}
class Signals
{
public:
    Map map;
    int x,y;
    bool go;
    bool leftTurn;

    void Initialize();
    void Draw(int x,int y) const;
    void DrawTurn(double x, double y, double angle) const;
    
};
void Signals::Initialize()
{
    go = false;
    leftTurn = false;
}

void Signals::Draw(int x,int y) const
{
    int W = 4, rad = 1;
    glColor3ub(0,0,0);
    glBegin(GL_QUADS);
    glVertex2i(10*x,windowH-10*y);
    glVertex2i(10*x,windowH-10*(y-W));
    glVertex2i(10*(x+W),windowH-10*(y-W));
    glVertex2i(10*(x+W),windowH-10*y);
    glEnd();
    if(go==true) glColor3ub(0,200,0);
    else glColor3ub(200,0,0);
    DrawCircle(10*(x+W/2),windowH-10*(y-W/2),10*rad,1);
    glEnd();
    
}
void Signals::DrawTurn(double x,double y, double angle) const
{
    double x1 = x+1.5,x2=x+1.5,x3 = x+1.5,x4=x+1.5,x5=x+3,x6=x+3;
    double y1 = y-1.5,y2=y+1.5,y3 = y-0.75,y4=y+0.75,y5=y-0.75,y6=y+0.75;

    vector<double> point1 = RotatePoint({x,y},{x1,y1},angle);
    vector<double> point2 = RotatePoint({x,y},{x2,y2},angle);
    vector<double> point3 = RotatePoint({x,y},{x3,y3},angle);
    vector<double> point4 = RotatePoint({x,y},{x4,y4},angle);
    vector<double> point5 = RotatePoint({x,y},{x5,y5},angle);
    vector<double> point6 = RotatePoint({x,y},{x6,y6},angle);

    if(leftTurn==true) glColor3ub(0,200,0);
    else glColor3ub(200,0,0);
    
    glBegin(GL_TRIANGLES);
    glVertex2d(10*x,windowH-10*y);    
    glVertex2d(10*point1[0],windowH-10*point1[1]);
    glVertex2d(10*point2[0],windowH-10*point2[1]);
    glEnd();
    
    glBegin(GL_QUADS);
    glVertex2d(10*point3[0],windowH-10*(point3[1]));
    glVertex2d(10*point5[0],windowH-10*(point5[1]));
    glVertex2d(10*point6[0],windowH-10*(point6[1]));
    glVertex2d(10*point4[0],windowH-10*(point4[1]));
    glEnd();

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
    vector<Signals> signals {sig1,sig2,sig3,sig4};
    for(auto &s: signals)
    {
        s.Initialize();
    }
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

    vector<Car*> cars_pointers;
    int car_number = 5;
    vector<double> checkPoint = map.center;

    bool create_car = false;

    vector<bool> collision_check;
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
        if(tPassed>70)
        {
            break;
        }

        // vector<int> decide = {tPassed%3,(tPassed+rand())%3,)}

        if(tPassed%6 == 0 && create_car == false){
            cout << "creating a car\n";
            Car* new_car = new Car();
            new_car->Initialize(car_number++, rand()%4+1);
            cars_pointers.push_back(new_car);
            create_car = true;
            collision_check.push_back(false);
        }

        if(create_car){
            if(tPassed%6 != 0){
                create_car = false;
            }
        }
        
        if(tPassed%14<7)
        {
            sig1.go = true,sig2.go=true,sig3.go=false,sig4.go=false;
            
        }
        else if(tPassed%12 > 6)
        {
            sig1.go = false,sig2.go=false,sig3.go=true,sig4.go=true;
        }
        
            
        

        
        int whereTo = tPassed%2;
        bool car_in_lane4 = false;
        for(auto &c : cars_pointers)
        {
            c->Turn(whereTo);
            // c->Turn(1);
            c->Move();
            if(c->x > map.U2[0]+10 && c->laneNum==4)
            {
                speed = c->vel * 3;
                car_in_lane4 = true;
            }
            if (Distance({c->x,c->y},checkPoint)<13)
            {
                
                if(sig3.go==false)
                {
                    if(c->laneNum==3 && c->x < checkPoint[0] - 12)
                    {
                        c->vel=0;
                    }
                    else if(c->laneNum==4 && c->x > checkPoint[0] + 12)
                    {
                        c->vel=0;
                    }
                }
                else if(sig1.go==false)
                {
                    if(c->laneNum==1 && c->y > checkPoint[1] + 12)
                    {
                        c->vel=0;
                    }
                    else if(c->laneNum==2 && c->y < checkPoint[1] - 12)
                    {
                        c->vel=0;
                    }
                
                }
                
            }
        }
        if(car_in_lane4 == false){
            speed = 0;
        }
        map.angle+=0.02;

        for(int i=0;i<cars_pointers.size();++i){
            if(cars_pointers[i]->check_collision(cars_pointers)){
                // cout << "collision!\n";
            }
        }

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 
        map.DrawBackGround();
        map.DrawRoads();
        map.DrawSigns(map.U2[0]+10,map.R2[1]+15);
        map.DispVel(map.U2[0]+10+2,map.R2[1]+15 - 2,speed);
        map.DrawSigns(map.D1[0]-25,map.L2[1]-5);
        map.DispWarning(map.D1[0]-25 + 2,map.L2[1]-5 - 2);
        map.DrawTree(15,55);
        map.DrawTree(20,65);
        map.DrawTree(30,60);
        map.DrawTree(35,50);
        map.DrawWindMill(65,15);
        map.DrawWindMill(77,20);
        map.DrawWindMill(88,30);

        sig1.Draw(map.centerX-10,map.centerY+10);
        // sig1.DrawTurn(map.centerX-6,map.centerY+14,YS_PI);
        sig2.Draw(map.centerX+6,map.centerY-6);
        // sig2.DrawTurn(map.centerX +6,map.centerY-14,0);
        sig3.Draw(map.centerX-10,map.centerY-6);
        // sig3.DrawTurn(map.centerX - 14,map.centerY-6,-YS_PI/2);
        sig4.Draw(map.centerX+6,map.centerY+10);
        // sig4.DrawTurn(map.centerX + 14,map.centerY + 6,YS_PI/2);
        for(auto &l1 : map.line1Vert)
        {
            map.DrawLines(l1[0],l1[1],1);
        }
        for(auto &l2 : map.line2Vert)
        {
            map.DrawLines(l2[0],l2[1],2);
        }
        

        for(int i=0;i<cars_pointers.size();++i){
            cars_pointers[i]->Draw();
        }

        FsSwapBuffers();
        FsSleep(25);
    }

    for(int i=0;i<cars_pointers.size();++i){
        delete cars_pointers[i];
    }
}   
