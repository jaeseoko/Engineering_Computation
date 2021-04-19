#include <stdio.h>
#include "fssimplewindow.h"
#include <iostream>
#include <math.h>
#include <vector>
using namespace std;

double const YS_PI = 3.1415927;

vector<double> navigation(double winddir, double windvel, double desiredc,double airspeed)
{
    vector<double> mymap;
    double theta_rw = (winddir + 180 - desiredc)*(YS_PI/180); // in radians
    double theta_wca = asin((-windvel*(sin(theta_rw)))/airspeed); // in radians
    double theta_H = (desiredc*(YS_PI/180) + theta_wca)*(180/YS_PI); // in degrees
    double vel_g = airspeed*cos(theta_wca) + windvel*cos(theta_rw);
    
    mymap.push_back(theta_H);
    mymap.push_back(vel_g);
    return mymap;
}
void DrawCircle(int cx,int cy,int rad,int fill)
{
    glColor3f(0.0,0.0,0.0);   
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

void DrawLine(int cx, int cy, double angle, double speed, float c1,float c2,float c3)
{   
    // angle = angle - (YS_PI/2);
    // Check angles should be in radians.
    double len = 3.0 * (speed);
    double x2 = cx + (sin(angle))*len;
    double y2 = cy - (cos(angle))*len;

    glColor3f(c1,c2,c3);
    glBegin(GL_LINES);
    glVertex2d(x2,y2);
    glVertex2d(400.0,300.0);
    glEnd();
}

int main(void)
{
    // double winddir = 220;
    // double windvel = 15;
    // double desiredc = 235;
    // double airspeed = 125;
    double winddir, windvel, desiredc, airspeed;
    cout << "\nENTER wind direction(angle), wind velocity," <<"\n";
    cout << "desired course(angle), air speed > ";
    cin >> winddir >> windvel >> desiredc >> airspeed;
    
    vector<double> map = navigation(winddir,windvel,desiredc,airspeed);
    double theta_H = map[0];
    double vel_g = map[1];
    
    cout << "\n\nWind Direction: " << winddir << endl;
    cout << "Wind Velocity: " << windvel << endl;
    cout << "Desired Course: " << desiredc << endl;
    cout << "Airspeed: " << airspeed << endl;
    cout << "\nHeading Angle should be: " << theta_H << endl;
    cout << "Expected ground speed is: " << vel_g << "\n\n";

    // SIMULATION
    FsOpenWindow(16,16,800,600,0);

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    // Draw Initial circle for the air plane position
    DrawCircle(400,300,100,0);
    // wind in red line.
    DrawLine(400,300,winddir*(YS_PI/180), windvel,1.0,0.0,0.0); 
    // Desired course in Blue line.
    DrawLine(400,300,desiredc*(YS_PI/180), vel_g,0.0,0.0,1.0);
    // Airplane heading in Green line.
    DrawLine(400,300,theta_H*(YS_PI/180), airspeed,0.0,0.7,0.0);
    glFlush();

    FsPollDevice();
    while(FSKEY_NULL==FsInkey())
    {
        FsPollDevice();
        FsSleep(25);
    }
    return 0;
}