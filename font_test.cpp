#include <stdio.h>

#include "fssimplewindow.h"
#include "ysglfontdata.h"



int main(void)
{
	FsOpenWindow(0,0,800,600,1);

	int ctr=0;
	for(;;)
	{
		FsPollDevice();
		auto key=FsInkey();
		if(FSKEY_ESC==key)
		{
			break;
		}
		++ctr;

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		glColor3ub(0,0,0);
		glRasterPos2i(100,100);
		YsGlDrawFontBitmap12x16("FONT RENDERING EXAMPLE");


		char str[256];
		sprintf(str,"%d",ctr);
		glRasterPos2i(100,116);
		YsGlDrawFontBitmap12x16(str);

		FsSwapBuffers();

		FsSleep(25);
	}
	return 0;
}
