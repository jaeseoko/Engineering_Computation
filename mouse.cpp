#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "fssimplewindow.h"
int main(void)
{
	FsOpenWindow(0,0,800,600,1);
	for(;;)
	{
		FsPollDevice();
		auto key=FsInkey();
		if(FSKEY_ESC==key)
		{
			break;
		}

		int lb,mb,rb,mx,my;
		auto evt=FsGetMouseEvent(lb,mb,rb,mx,my);
		if(FSMOUSEEVENT_LBUTTONDOWN==evt)
		{
			// Mouse left button pressed
		}

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		if(0!=lb)
		{
			glColor3ub(255,0,0);
		}
		else
		{
			glColor3ub(0,255,0);
		}

		glBegin(GL_QUADS);
		glVertex2i(mx-20,my);
		glVertex2i(mx,my-20);
		glVertex2i(mx+20,my);
		glVertex2i(mx,my+20);
		glEnd();

		FsSwapBuffers();

		FsSleep(10);
	}
	return 0;
}
