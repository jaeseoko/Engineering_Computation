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
const double PI=3.1415927;
const double Tol =1e-4;

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
        double angle=(double)i*PI/32.0;
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
    double angle = 0;
    
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
                                        
    vector<double> point1 = RotatePoint({x,y},{x1,y1},angle);
    vector<double> point2 = RotatePoint({x,y},{x2,y1},angle);
    vector<double> point3 = RotatePoint({x,y},{x1,y1},angle+2/3*PI);
    vector<double> point4 = RotatePoint({x,y},{x2,y1},angle+2/3*PI);
    vector<double> point5 = RotatePoint({x,y},{x1,y1},angle+4/3*PI);
    vector<double> point6 = RotatePoint({x,y},{x2,y1},angle+4/3*PI);
    
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
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2d(10*x,windowH-10*y);
    glVertex2d(10*point3[0],windowH-10*point3[1]);
    glVertex2d(10*point4[0],windowH-10*point4[1]);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2d(10*x,windowH-10*y);
    glVertex2d(10*point5[0],windowH-10*point5[1]);
    glVertex2d(10*point6[0],windowH-10*point6[1]);
    glEnd();
    

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
    else if(go==false) glColor3ub(200,0,0);
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
    else if(leftTurn==false) glColor3ub(200,0,0);
    
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

class Car
{
protected:
    double len = 3,width = 1.5, wheelR=0.5;
public:
    Map map;
    int turn, carNum;
    vector<double>turnP1 = {60,50};
    vector<double>turnP2 = {40,30};
    vector<double>turnP3 = {40,50};
    vector<double>turnP4 = {60,30};
    double turnRad = 12.5;
    double x,y,angle,vel,acc;
    int turnCount=0,exitCount = 0;
    
    int laneNum;
    bool leftTurn;
    void Initialize(int lane);        
    void Draw(int CarNum) const;
    void Move();
    bool check_near(const Car* other_car){
        if(pow(x-other_car->x,2)+pow(y-other_car->y,2)<20 && laneNum == other_car->laneNum)
        // if(pow(x-other_car->x,2)+pow(y-other_car->y,2)<pow(4,2))
        {
            cout << "x : " << x << " y : " << y << endl;
            // vel -= 5*brkacc*dt;
            vel = 0;

            return true;
        }
        vel += acc*dt;
        return false;
    }
    bool check_collision(const vector<Car*>& cars_pointers){
        for(int i=0;i<cars_pointers.size();++i){
            if(this == cars_pointers[i]) break;
            if(check_near(cars_pointers[i])){
                return true;
            }
        }
        return false;
    }
    
};
void Car::Initialize(int lane)
{
    carNum = 1;
    vel = 5,acc = 20;
    leftTurn = false;
    laneNum = lane;
    if(1==laneNum)
    {
        angle = 0;
        x=map.vertAx, y=80 +5;
        
    }
    else if(2==laneNum)
    {
        angle = 0;
        x=map.vertDx, y=0 - 5;
        
    }
    else if(3==laneNum)
    {
        angle = PI/2;
        x=0 - 5, y=map.vertBy;
        
    }
    else if(4==laneNum)
    {
        angle = PI/2;
        x=100 +5,y=map.vertDy;
    }
}
void Car::Draw(int carNum) const
{
    //FOR BODY
    vector<double> point1 = {x-width/2,y+len/2};
    vector<double> point2 = {x-width/2,y-len/2};
    vector<double> point3 = {x+width/2,y-len/2};
    vector<double> point4 = {x+width/2,y+len/2};
    // FOR TIRES
    vector<double> point5 = {x-width/2,y+width/2};
    vector<double> point6 = {x+width/2,y+width/2};
    vector<double> point7 = {x-width/2,y-width/2};
    vector<double> point8 = {x+width/2,y-width/2};
    vector<double> point0 = {x,y};
    vector<vector<double>> points = {point1,point2,point3,point4,point5,point6,point7,point8,point0};
    vector<vector<double>> newPoints {};
    if(leftTurn==false)
    {
        for(vector<double> p : points)
        {
            newPoints.push_back(RotatePoint({x,y},p,angle));
        }
    }    
    else if(leftTurn==true)
    {
        if(laneNum==1 && y < turnP1[1]) for(vector<double> p : points)
        {
            newPoints.push_back(RotatePoint(turnP1,p,angle));
        }
        else if(laneNum==2 && y > turnP2[1]) for(vector<double> p : points)
        {
            newPoints.push_back(RotatePoint(turnP2,p,angle));
        }
        else if(laneNum==3) for(vector<double> p : points)
        {
            newPoints.push_back(RotatePoint(turnP3,p,angle));
        }
        else if(laneNum==4) for(vector<double> p : points)
        {
            newPoints.push_back(RotatePoint(turnP4,p,angle));
        }
    }
    glColor3ub(0,0,0);
    DrawCircle(10*(newPoints[7][0]),windowH - 10*(newPoints[7][1]),10*0.5,1);
    DrawCircle(10*(newPoints[6][0]),windowH - 10*(newPoints[6][1]),10*0.5,1);
    DrawCircle(10*(newPoints[5][0]),windowH - 10*(newPoints[5][1]),10*0.5,1);
    DrawCircle(10*(newPoints[4][0]),windowH - 10*(newPoints[4][1]),10*0.5,1);
    glEnd();
    if(carNum%4==0) glColor3ub(255,0,0);
    else if(carNum%4==1) glColor3ub(255,255,0);
    else if(carNum%4==2) glColor3ub(0,120,255);
    else if(carNum%4==3) glColor3ub(255,255,255);
    glBegin(GL_QUADS);
    glVertex2d(10*(newPoints[0][0]),windowH - 10*(newPoints[0][1]));
    glVertex2d(10*newPoints[1][0],windowH - 10*(newPoints[1][1]));
    glVertex2d(10*(newPoints[2][0]),windowH - 10*(newPoints[2][1]));
    glVertex2d(10*newPoints[3][0],windowH - 10*newPoints[3][1]);
    glEnd();
    newPoints.clear(),points.clear();
    
}
void Car::Move()
{
    vel+=acc*dt;
    
    if(vel>25) vel = 25;
    if(vel<0) vel=0;

    if(y<0-len/2 || y>len/2 +myWindowH || x<0 -width/2|| x>myWindowW+width/2)
    {
        if(1==laneNum)      laneNum+=1,x = map.vertDx,y=0 - 5;
        else if(2==laneNum) laneNum+=1,x = 0 - 5        ,y=map.vertBy;
        else if(3==laneNum) laneNum+=1,x = 100 + 5       ,y=map.vertDy;
        else if(4==laneNum) laneNum=1 ,x = map.vertAx,y=80 + 5;
        exitCount++;
        turnCount=exitCount;
        
    }
    
    if(leftTurn==false)
    {
        if(laneNum==1)      y -= vel*dt,x=map.vertAx,angle = 0;
        else if(laneNum==2) y += vel*dt,x=map.vertDx,angle = 0; 
        else if(laneNum==3) x += vel*dt,y=map.vertBy,angle = PI/2; 
        else if(laneNum==4) x -= vel*dt,y=map.vertDy,angle = PI/2;
    }
    else if(leftTurn==true && turnCount == exitCount)
    {
        angle+=(vel/turnRad)*dt;
        


        // if (c->laneNum==1 && c->y < c->turnP1[1]) c->leftTurn=true;
        // else if(c->laneNum==2 && c->y > c->turnP2[1]) c->leftTurn=true;
        // else if(c->laneNum==3 && c->x > c->turnP3[0]) c->leftTurn=true;
        // else if(c->laneNum==4 && c->x < c->turnP4[0]) c->leftTurn=true;

        
        // if(laneNum==1)
        // {
        //     x = RotatePoint(turnP1,{x,y},angle)[0];
        //     y = RotatePoint(turnP1,{x,y},angle)[1];
        // }
        // else if(laneNum==2)
        // {
        //     x = RotatePoint(turnP2,{x,y},angle)[0];
        //     y = RotatePoint(turnP2,{x,y},angle)[1];
        // }
        // else if(laneNum==3)
        // {
        //     x = RotatePoint(turnP3,{x,y},angle)[0];
        //     y = RotatePoint(turnP3,{x,y},angle)[1];
        // }
        // else if(laneNum==4)
        // {
        //     x = RotatePoint(turnP4,{x,y},angle)[0];
        //     y = RotatePoint(turnP4,{x,y},angle)[1];
        // }
        if(angle>=PI/2)
        {
            if(laneNum==1 && x>turnP1[0]) laneNum+=2,x=turnP4[0];
            else if(laneNum==2 && x<turnP2[0]) laneNum+=2,x=turnP3[0];
            // laneNum+=2,angle = PI/2,
            leftTurn = false, turnCount++;
            
        }
        else if(angle>=PI)
        {
            
            if(laneNum==3 && y>turnP3[1]) laneNum-=1,y=turnP1[1];
            else if(laneNum==4 && y<turnP4[1]) laneNum-=1, y=turnP2[1];
            // laneNum-=1,angle = 0,
            leftTurn = false, turnCount++;
        } 
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
    vector<Signals> signals {sig1,sig2,sig3,sig4};
    for(auto &s: signals)
    {
        s.Initialize();
    }
    
    // srand(time(NULL));
    
    time_t start = time(0),breaking,breakingEnd;
    FsOpenWindow(16,16,windowW,windowH,1);
    player.Start();

    Car car1;
    car1.Initialize(2);
    
    vector<double> checkPoint = map.center;

    bool create_car = false;
    int turning = 0;
    vector<bool> collision_check;


    vector<Car*> cars_pointers;

    int car_number = 5;
    // srand(time(NULL));
    // Car car1,car2,car3,car4;
    // car1.Initialize(1);
    // car2.Initialize(2);
    // car3.Initialize(3);
    // car4.Initialize(4);
    // vector<Car> cars {car1,car2,car3,car4};

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

        if(tPassed%4==0 && create_car == false){
            cout << "creating a car\n";
            Car* new_car = new Car();
            new_car->Initialize(1);
            new_car->carNum= rand()%4+1;
            cars_pointers.push_back(new_car);
            create_car = true;
            // collision_check.push_back(false);
        }
        
        if(create_car){
            if(tPassed%4!=0){
                create_car = false;
            }
        }
        
        if(tPassed%12<6)
        {
            sig1.go = true,sig2.go=true,sig3.go=false,sig4.go=false;
            
        }
        else if(tPassed%12 > 6)
        {
            sig1.go = false,sig2.go=false,sig3.go=true,sig4.go=true;
        }
        


        bool car_in_lane4 = false;
        // int whereTo = rand()%3;
        int whereTo = rand()%2;
    
        for(auto &c : cars_pointers)
        {
            
            c->Move();

            // if(sig3.go ==false ||sig4.go==false && Distance({c->x,c->y},checkPoint)<12)
            if(sig3.go ==false ||sig4.go==false)
            {
                if(c->laneNum==3 && c->x > c->turnP3[0])
                {
                    c->vel=0;
                }
                else if(c->laneNum==4 && c->x < c->turnP4[0])
                {
                    c->vel=0;
                }
            }
            else if(sig1.go==false || sig2.go==false && Distance({c->x,c->y},checkPoint)>12)
            {
                if(c->laneNum==1 && c->y < c->turnP1[1])
                {
                    c->vel=0;
                }
                else if(c->laneNum==2 && c->y > c->turnP2[1])
                {
                    c->vel=0;
                }
            }

            if(c->x > map.U2[0]+10 && c->laneNum==4)
            {
                speed = c->vel * 3;
                car_in_lane4 = true;
            }
            // 0 = STRAIGHT , 1 = RIGHT TURN, 2 = LEFT TURN
            if(whereTo==1 && c->turnCount == c->exitCount)
            {
                
                if(1==c->laneNum && sig1.go==true)
                {
                    if(c->y<45) c->laneNum = 4,c->turnCount++;
                }
                else if(2==c->laneNum&& sig2.go==true)
                {
                    if(c->y>35) c->laneNum = 3,c->turnCount++;
                }
                else if(3==c->laneNum&& sig3.go==true)
                {
            
                    if(c->x>45) c->laneNum = 1,c->turnCount++;
                
                }
                else if(4==c->laneNum && sig4.go==true)
                {
            
                    if(c->x<55) c->laneNum = 2,c->turnCount++;
                
                }
                
            }
            else if(whereTo==2 && c->turnCount == c->exitCount)
            {
                cout<< "x,y = "<< c->x << ", "  << c->y <<endl;
                if     (c->laneNum==1 && c->y < c->turnP1[1] && sig1.leftTurn==true) c->leftTurn=true;
                else if(c->laneNum==2 && c->y > c->turnP2[1] && sig2.leftTurn==true) c->leftTurn=true;
                else if(c->laneNum==3 && c->x > c->turnP3[0] && sig3.leftTurn==true) c->leftTurn=true;
                else if(c->laneNum==4 && c->x < c->turnP4[0] && sig4.leftTurn==true) c->leftTurn=true;
            }
            
            
        }
        if(car_in_lane4 == false){
            speed = 0;
        }
        
   
        map.angle+=0.02;
        


        
        for(int i=0;i<cars_pointers.size();++i){
            if(cars_pointers[i]->check_collision(cars_pointers)){
                cout << "collision!\n";
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
        // sig1.DrawTurn(map.centerX-6,map.centerY+14,PI);
        sig2.Draw(map.centerX+6,map.centerY-6);
        // sig2.DrawTurn(map.centerX +6,map.centerY-14,0);
        sig3.Draw(map.centerX-10,map.centerY-6);
        // sig3.DrawTurn(map.centerX - 14,map.centerY-6,-PI/2);
        sig4.Draw(map.centerX+6,map.centerY+10);
        // sig4.DrawTurn(map.centerX + 14,map.centerY + 6,PI/2);
        for(auto &l1 : map.line1Vert)
        {
            map.DrawLines(l1[0],l1[1],1);
        }
        for(auto &l2 : map.line2Vert)
        {
            map.DrawLines(l2[0],l2[1],2);
        }
        glColor3ub(255,255,255);
        
        for(int i=0;i<cars_pointers.size();++i){
            cars_pointers[i]->Draw(cars_pointers[i]->carNum);
        }
        

        glEnd();
        
        FsSwapBuffers();
        FsSleep(25);
    }
    for(int i=0;i<cars_pointers.size();++i)
    {
        delete cars_pointers[i];
    }
}   
