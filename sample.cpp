#include <stdio.h>
#include "fssimplewindow.h"

int main(void)
{
    FsOpenWindow(16,16,800,600,0);

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glColor3f(0.0,0.0,1.0);
    glBegin(GL_LINES);
    glVertex2i(0,0);
    glVertex2i(800,600);
    glEnd();

    glFlush();

    FsPollDevice();
    while(FSKEY_NULL==FsInkey())
    {
        FsPollDevice();
        FsSleep(25);
    }

    return 0;
}
