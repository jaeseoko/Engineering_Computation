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

const int windowW = 1000,windowH = 600;
const double PI = 3.141592;
const double dt = 0.025;
const double Tol = 1e-4;



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

void DrawArrow(double x,double y, double angle)
{
    double x1 = x+1,x2=x+1,x3 = x+1,x4=x+1,x5=x+2,x6=x+2;
    double y1 = y-1,y2=y+1,y3 = y-0.5,y4=y+0.5,y5=y-0.5,y6=y+0.5;

    vector<double> point1 = RotatePoint({x,y},{x1,y1},angle);
    vector<double> point2 = RotatePoint({x,y},{x2,y2},angle);
    vector<double> point3 = RotatePoint({x,y},{x3,y3},angle);
    vector<double> point4 = RotatePoint({x,y},{x4,y4},angle);
    vector<double> point5 = RotatePoint({x,y},{x5,y5},angle);
    vector<double> point6 = RotatePoint({x,y},{x6,y6},angle);
    
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

void DrawCircle(double cx,double cy,double rad,int fill, int R,int G,int B)
{
    glColor3ub(R,G,B);
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
void DrawRectangle(double x, double y, double len, double hei,int R,int G,int B)
{
    glColor3ub(R,G,B);
    glBegin(GL_QUADS);
    glVertex2d(10*x,windowH - 10*y);
    glVertex2d(10*(x + len),windowH - 10*y);
    glVertex2d(10*(x + len),windowH - 10*(y+hei));
    glVertex2d(10*x,windowH - 10*(y+hei));
    glEnd();
}




class Map
{
public:
    double startX = 30, startY = 5;
    void Draw() const;
    void DrawSign() const;
    void DrawReady() const;
    void DrawSet() const;
    void DrawGo() const;
    void DrawStartLine() const;
};

void Map::Draw() const
{
    // Background
    glColor3ub(23,141,19);
    glBegin(GL_QUADS);
    glVertex2i(0,0);
    glVertex2i(windowW,0);
    glVertex2i(windowW,windowH);
    glVertex2i(0,windowH);
    glEnd();
    //
    DrawCircle(10*60,windowH-10*25,10*25,1,75,75,75);
    DrawCircle(10*60,windowH-10*25,10*20,1,23,141,19);
    //
    DrawCircle(10*30,windowH-10*10,10*10,1,75,75,75);
    DrawCircle(10*30,windowH-10*10,10*5,1,23,141,19);
    //
    DrawCircle(10*50,windowH-10*25,10*10,1,75,75,75);
    DrawCircle(10*50,windowH-10*25,10*5,1,23,141,19);
    //
    DrawCircle(10*30,windowH-10*40,10*10,1,75,75,75);
    DrawCircle(10*30,windowH-10*40,10*5,1,23,141,19);
    glEnd();
    //
    DrawRectangle(30,0,30,5,75,75,75);
    DrawRectangle(30,15,20,5,75,75,75);
    DrawRectangle(30,30,20,5,75,75,75);
    DrawRectangle(30,45,30,5,75,75,75);
    //
    DrawRectangle(30,5,30,10,23,141,19);
    DrawRectangle(30,20,20,10,23,141,19);
    DrawRectangle(30,35,30,10,23,141,19);
    //
    glColor3ub(255,255,0);
    DrawArrow(45,2.5,0);

    glColor3ub(0,0,0);
    glRasterPos2d(10*30,windowH - 10*55);
    YsGlDrawFontBitmap20x32("2020 GRAND PRIX MONACO");
    
}
void Map::DrawSign() const
{
    DrawRectangle(35,6.5,2,6,0,0,0);
}
void Map::DrawReady() const
{
    
    DrawCircle(10*36,windowH - 10*11.5,10*0.5,1,255,0,0);
    glEnd();
}
void Map::DrawSet() const
{    
    
    DrawCircle(10*36,windowH - 10*9.5,10*0.5,1,255,255,0);
    glEnd();
}
void Map::DrawGo() const
{
    
    DrawCircle(10*36,windowH - 10*7.5,10*0.5,1,0,200,0);
    glEnd();
}
void Map::DrawStartLine() const
{
    // startX = 30, startY = 5;
    double lineLen = 1,gap=1;
    glColor3ub(255,255,255);
    glBegin(GL_LINES);
    glVertex2d(10*startX,windowH - 10*startY);
    glVertex2d(10*startX,windowH - 10*(startY-lineLen));

    glVertex2d(10*startX,windowH - 10*(startY-lineLen-gap));
    glVertex2d(10*startX,windowH - 10*(startY-2*lineLen-gap));

    glVertex2d(10*startX,windowH - 10*(startY-2*lineLen-2*gap));
    glVertex2d(10*startX,windowH - 10*(startY-3*lineLen-2*gap));
    glEnd();
}


class Car
{
protected:
    double len = 2,width = 1, wheelR=0.3;
public:
    Map map;
    int turn, carNum;
    int gap = 9;
    double startT,endT;
    int lapCount;
    double lapTime;
    double travelD;
    double checkP1x, checkP1y;
    double checkP2x, checkP2y;

    bool checkPoint1,checkPoint2;

    double turnRad = 12.5;
    double x,y,angle,vel,acc,maxVel;
    int turnCount=0,exitCount = 0;
    double turnRad1 = 6.25,turnRad0 = 8.75;
    double turnRad11 = 21.25,turnRad00 = 23.75;
    int laneNum;
    bool leftTurn;
    void Initialize(int carN,int lane, double xPos);        
    void Draw() const;
    void Move();
    // WRITE THIS -> ########################################################################
    void DrawLapBoard() const;
    void DisplayLap() const;
    double getTravelD();
    void countLaps();
    void countLapTime();
    
};
void Car::Initialize(int carN,int lane, double xPos)
{
    checkP1x = map.startX + 5, checkP1y = map.startY+Tol;
    checkP2x = map.startX+20, checkP2y = map.startY +Tol;
    checkPoint1 = false;
    checkPoint2 = false;
    travelD = 0;
    startT = 0,endT = 0;
    lapCount=-1,lapTime=0;
    carNum = carN;
    laneNum = lane;
    acc = 0.4 + rand()%3/100;
    vel = 0;
    maxVel = 4;
    angle = 0;
    if(lane==0)x = xPos;
    else if(lane==1) x= xPos + gap;
    
    if(lane==0)
    {
        y = 1.25;
    }
    else y = 3.75;
}


void Car::Draw() const
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
    
    for(vector<double> p : points)
    {
        newPoints.push_back(RotatePoint({x,y},p,-angle-PI/2));
    }

    DrawCircle(10*(newPoints[7][0]),windowH - 10*(newPoints[7][1]),10*0.5,1,0,0,0);
    DrawCircle(10*(newPoints[6][0]),windowH - 10*(newPoints[6][1]),10*0.5,1,0,0,0);
    DrawCircle(10*(newPoints[5][0]),windowH - 10*(newPoints[5][1]),10*0.5,1,0,0,0);
    DrawCircle(10*(newPoints[4][0]),windowH - 10*(newPoints[4][1]),10*0.5,1,0,0,0);
    glEnd();
    if(carNum==1) glColor3ub(255,0,0);
    else if(carNum==2) glColor3ub(255,255,0);
    else if(carNum==3) glColor3ub(0,120,255);
    else if(carNum==4) glColor3ub(255,255,255);
    else if(carNum==5) glColor3ub(0,150,50);
    else if(carNum==6) glColor3ub(106,13,173);
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
    
    vel += acc*dt;
    if(vel>maxVel) vel = maxVel;
    x   -= vel*cos(angle);
    y   += vel*sin(angle);

    // STRAIGHT
    if(x>30 && x<60 && y<5)
    {
        maxVel = 4;
        if(laneNum==1) y = 3.75;
        else if(laneNum==0) y = 1.25;
        angle = 0;
    }
    else if(x>30 && x<50 && y>15 && y<20)
    {
        maxVel = 4;
        if(laneNum==1) y = 16.25;
        else if(laneNum==0) y = 18.75;
        angle = PI;
    }
    else if(x>30 && x<50 && y>30 && y<35)
    {
        maxVel = 4;
        if(laneNum==1) y = 33.75;
        else if(laneNum==0) y = 31.25;
        angle = 0;
    }
    else if(x>30 && x<60 && y>45 && y<50)
    {
        maxVel = 4;
        if(laneNum==1) y = 46.25;
        else if(laneNum==0) y = 48.75;
        angle = PI;
    }
    // <- STRAIGHT

    // CORNERING ->
    if(x<30-Tol)
    {
        maxVel = 1.5;
        if(laneNum==1) angle += vel/turnRad1;
        else angle+= vel/turnRad0; 
        if(angle>=PI) angle = PI;
    }
    else if(x>=50 && y<=40 && x<60)
    {
        maxVel = 1.5;
        if(laneNum==1) angle -= vel/turnRad0;
        else angle-= vel/turnRad1; 
        if (angle<=0) angle = 0;
    }
    else if(x>=60 - Tol)
    {
        maxVel = 1.5;
        if(laneNum==1) angle += vel/turnRad11;
        else angle += vel/turnRad00; 
        if(angle<=0) angle = 0;
    }
    // <- CORNERING 
}

void Car::DrawLapBoard() const
{
    DrawRectangle(3,10,15,40,0,0,0);
    glColor3ub(255,255,255);
    glRasterPos2d(10*3.5,windowH - 10*47);
    YsGlDrawFontBitmap16x20("LAP TIMES");
    glColor3ub(255,255,255);
    glRasterPos2d(10*6,windowH - 10*29);
    YsGlDrawFontBitmap16x20("TOTAL");
    glRasterPos2d(10*6.8,windowH - 10*26);
    YsGlDrawFontBitmap16x20("LAPS");
}
void Car::DisplayLap() const
{
    double gap = 1.5,len=1.5,hei=1, dispX = 10;
    if(carNum==1) DrawRectangle(4,44,len,hei,255,0,0),DrawRectangle(4,23,len,hei,255,0,0);
    else if(carNum==2) DrawRectangle(4,44 - gap - hei,len,hei,255,255,0),DrawRectangle(4,23 - gap - hei,len,hei,255,255,0);
    else if(carNum==3) DrawRectangle(4,44 - 2*gap - 2*hei,len,hei,0,120,255),DrawRectangle(4,23 - 2*gap - 2*hei,len,hei,0,120,255);
    else if(carNum==4) DrawRectangle(4,44 - 3*gap - 3*hei,len,hei,255,255,255),DrawRectangle(4,23 - 3*gap - 3*hei,len,hei,255,255,255);
    else if(carNum==5) DrawRectangle(4,44 - 4*gap - 4*hei,len,hei,0,150,50),DrawRectangle(4,23 - 4*gap - 4*hei,len,hei,0,150,50);
    else if(carNum==6) DrawRectangle(4,44 - 5*gap - 5*hei,len,hei,106,13,173),DrawRectangle(4,23 - 5*gap - 5*hei,len,hei,106,13,173);

    glColor3ub(255,255,255);
    if(carNum==1)
    {
        glRasterPos2d(10*dispX,windowH - 10*44);
        char str[256];
        sprintf(str,"%f",lapTime);
        YsGlDrawFontBitmap8x12(str);

        glRasterPos2d(10*dispX,windowH - 10*23);
        char count[256];
        sprintf(count,"%d",(int)lapCount);
        if(lapCount<0) YsGlDrawFontBitmap8x12("0");
        else YsGlDrawFontBitmap8x12(count);
        
    }
    else if(carNum==2)
    {
        glRasterPos2d(10*dispX,windowH - 10*(44- gap - hei));
        char str[256];
        sprintf(str,"%d",(int)lapTime);
        YsGlDrawFontBitmap8x12(str);

        glRasterPos2d(10*dispX,windowH - 10*(23- gap - hei));
        char count[256];
        sprintf(count,"%d",(int)lapCount);
        if(lapCount<0) YsGlDrawFontBitmap8x12("0");
        else YsGlDrawFontBitmap8x12(count);
        
    } 
    else if(carNum==3)
    {
        glRasterPos2d(10*dispX,windowH - 10*(44- 2*gap - 2*hei));
        char str[256];
        sprintf(str,"%d",(int)lapTime);
        YsGlDrawFontBitmap8x12(str);

        glRasterPos2d(10*dispX,windowH - 10*(23- 2*gap - 2*hei));
        char count[256];
        sprintf(count,"%d",(int)lapCount);
        if(lapCount<0) YsGlDrawFontBitmap8x12("0");
        else YsGlDrawFontBitmap8x12(count);
        
    }  
    else if(carNum==4)
    {
        glRasterPos2d(10*dispX,windowH - 10*(44- 3*gap - 3*hei));
        char str[256];
        sprintf(str,"%d",(int)lapTime);
        YsGlDrawFontBitmap8x12(str);

        glRasterPos2d(10*dispX,windowH - 10*(23- 3*gap - 3*hei));
        char count[256];
        sprintf(count,"%d",(int)lapCount);
        if(lapCount<0) YsGlDrawFontBitmap8x12("0");
        else YsGlDrawFontBitmap8x12(count);
        
    } 
    else if(carNum==5)
    {
        glRasterPos2d(10*dispX,windowH - 10*(44- 4*gap - 4*hei));
        char str[256];
        sprintf(str,"%d",(int)lapTime);
        YsGlDrawFontBitmap8x12(str);

        glRasterPos2d(10*dispX,windowH - 10*(23- 4*gap - 4*hei));
        char count[256];
        sprintf(count,"%d",(int)lapCount);
        if(lapCount<0) YsGlDrawFontBitmap8x12("0");
        else YsGlDrawFontBitmap8x12(count);
        
    } 
    else if(carNum==6)
    {
        glRasterPos2d(10*dispX,windowH - 10*(44- 5*gap - 5*hei));
        char str[256];
        sprintf(str,"%d",(int)lapTime);
        YsGlDrawFontBitmap8x12(str);

        glRasterPos2d(10*dispX,windowH - 10*(23- 5*gap - 5*hei));
        char count[256];
        sprintf(count,"%d",(int)lapCount);
        if(lapCount<0) YsGlDrawFontBitmap8x12("0");
        else YsGlDrawFontBitmap8x12(count);
        
    }    
}
double Car::getTravelD()
{
    travelD += vel*sqrt(x*x+y*y)/1000;
    return travelD;
}
void Car::countLaps()
{
    if(x<checkP1x && y<checkP1y && checkPoint1==false)
    {
        checkPoint1 = true, lapCount++;
        cout << "car has passed" << endl;
        // cout << lapCount << endl;
    }
    if(x>checkP2x && y<checkP2y && checkPoint1 == true)
    {
        checkPoint1 = false;
    }
}
void Car::countLapTime()
{   
    
}


// Timer tmr;
//     double t = tmr.elapsed();
//     std::cout << t << std::endl;

//     tmr.reset();
//     t = tmr.elapsed();
//     std::cout << t << std::endl;
//     return 0;


// const clock_t begin_time = clock();
// // do something
// std::cout << float( clock () - begin_time ) /  CLOCKS_PER_SEC;



int main(void)
{
    YsSoundPlayer player;
    YsSoundPlayer::SoundData GrandPrixSE;


    Map map;
    Car car1,car2,car3,car4,car5,car6;
    
    
    car1.Initialize(1,1,35);
    car2.Initialize(2,0,35);
    car3.Initialize(3,1,42);
    car4.Initialize(4,0,42);
    car5.Initialize(5,1,49);
    car6.Initialize(6,0,49);
    vector<Car*> cars {};
    cars.push_back(&car1),cars.push_back(&car2),cars.push_back(&car3);
    cars.push_back(&car4),cars.push_back(&car5),cars.push_back(&car6);
    
    double totalT1=0,totalT2=0,totalT3=0,totalT4=0,totalT5=0,totalT6=0;
    vector<double> totalTimes = {totalT1,totalT2,totalT3,totalT4,totalT5,totalT6};
    time_t begin, end;
    time_t start = time(0);
    FsOpenWindow(16,16,windowW,windowH,1);
    FsChangeToProgramDir();
	if(YSOK!=GrandPrixSE.LoadWav("GrandPrixSE.wav"))
	{
		printf("Load error.\n");
		return 1;
	}


    for(;;)
    {
        

        FsPollDevice();
        auto key=FsInkey();
        
        
        
        int tPassed = difftime(time(0), start);
        double tPassed2 = difftime(time(0),start);

        player.KeepPlaying();
        if(tPassed>12) player.PlayBackground(GrandPrixSE);
        if(FSKEY_ESC==key)
        {
            break;
        }
        if(tPassed>85)
        {
            break;
        }

        // if(tPassed>5) car1.Move(), car2.Move();
        // if(tPassed>10) car3.Move(),car4.Move();
        // if(tPassed>15) car5.Move(),car6.Move();

        for(int i=0; i<cars.size(); ++i)
        {
            if(tPassed>5) cars[i]->Move();
            
        }



        // for(auto &c : cars)
        // {
        //     if(c->x<c->checkP1x && c->y<c->checkP1y && c->checkPoint1==false)
        //     {
        //         c->checkPoint1 = true, c->lapCount++;
        //         cout << c->lapCount << endl;
        //     }
        //     if(c->x>c->checkP2x && c->y<c->checkP2y && c->checkPoint1 == true)
        //     {
        //         c->checkPoint1 = false;
        //     }
        // }

        for(auto &c : cars) c->countLaps(); 
        // cout<< c->lapCount << endl;

        // for(int i=0; i<cars.size(); ++i)
        // {
        //     if(cars[i]->x>cars[i]->checkP2x && cars[i]->y<cars[i]->checkP2y && cars[i]->checkPoint1 == false) 
        //     {
        //         cout << "CHECKKKKKKKKKKKKK     1" << endl;
        //         if(cars[i]->lapCount<=0)
        //         {
        //             cout << "CHECKKKKKKKKKKKKK     2" << endl;
        //             cars[i]->lapTime = tPassed2 - 5;
        //             cout << cars[i]->lapTime <<endl;
        //         }
        //         else if(cars[i]->lapCount>0) cars[i]->lapTime = tPassed2 - totalTimes[i];
        //         totalTimes[i] += cars[i]->lapTime;
        //     }
        // }


        // if(x<checkP1x && y<checkP1y && checkPoint1==false)
        // {
        //     checkPoint1 = true, lapCount++;
        // // cout << lapCount << endl;
        // }
        // if(x>checkP2x && y<checkP2y && checkPoint1 == true)
        // {
        //     checkPoint1 = false;
        // }

        // car1.countLaps(),car2.countLaps();
        // car3.countLaps(),car4.countLaps();
        // car5.countLaps(),car6.countLaps();

        
        
       
        

        

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 
        map.Draw();
        map.DrawSign();
        map.DrawStartLine();
        if(tPassed<3) map.DrawReady();
        if(tPassed>3 && tPassed<5) map.DrawSet();
        if(tPassed>5) map.DrawGo();
        car1.DrawLapBoard();
        for(auto &c: cars) c->Draw(),c->DisplayLap();
        // car1.Draw(),car2.Draw();
        // car3.Draw(),car4.Draw();
        // car5.Draw(),car6.Draw();
        
        
        // car1.DisplayLap(),car2.DisplayLap();
        // car3.DisplayLap(),car4.DisplayLap();
        // car5.DisplayLap(),car6.DisplayLap();
        // for(auto &c : cars) c.DisplayLap();
        FsSwapBuffers();
        FsSleep(25);



    }


}