#include <stdio.h>
#include <stdlib.h>

#include "fssimplewindow.h"

void PlotXSquare(double a)
{
    glColor3ub(0,0,0);
    glBegin(GL_LINE_STRIP);

    double x;
    for(x=0.0; x<8.0; x+=0.01)
    {
        const double y=a*x*x;
        glVertex2d(x*100.0,600.0-y*100.0);

    }
    glEnd();
}
int main(void)
{
    double a;

    FsOpenWindow(16,16,800,600,0);

    glClear(GL_COLOR_BUFFER_BIT|
            GL_DEPTH_BUFFER_BIT);

    for(a=0.1; a<1.0; a+=0.1)
    {
        PlotXSquare(a);
        glFlush();
        while(FSKEY_NULL==FsInkey())
        {
            FsPollDevice();
            FsSleep(10);
        }
    }
    return 0;
}