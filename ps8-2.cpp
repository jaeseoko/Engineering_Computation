#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "fssimplewindow.h"



class Parser
{
protected:
	char *str;
	int nw,*wTop,*wLen;
public:
	Parser();
	~Parser();
	void clear(void);
	int Parse(char input[]);

	int GetNumWord(void);
	void GetWord(char dst[],int maxlen,int i);
};
Parser::Parser()
{
	str=nullptr;
	wTop=nullptr;
	wLen=nullptr;
	nw=0;
}
Parser::~Parser()
{
	clear();
}
void Parser::clear(void)
{
	if(nullptr!=str)
	{
		delete [] str;
		str=nullptr;
	}
	if(nullptr!=wTop)
	{
		delete [] wTop;
		wTop=nullptr;
	}
	if(nullptr!=wLen)
	{
		delete [] wLen;
		wLen=nullptr;
	}
	nw=0;
}
int Parser::Parse(char input[])
{
	clear();

	int strl=strlen(input);
	str=new char [strl+1];
	strcpy(str,input);

	wTop=new int [(strl+1)/2];
	wLen=new int [(strl+1)/2];

	nw=0;

	int state=0;
	for(int i=0; 0!=input[i]; ++i)
	{
		auto c=input[i];
		if(0==state)
		{
			if(' '==c || '\t'==c || '\n'==c || '\r'==c)
			{
			}
			else
			{
				state=1;
				wTop[nw]=i;
				wLen[nw]=1;
				++nw;
			}
		}
		else // if(1==state)
		{
			if(' '==c || '\t'==c || '\n'==c || '\r'==c)
			{
				state=0;
			}
			else
			{
				++wLen[nw-1];
			}
		}
	}
	return nw;
}
int Parser::GetNumWord(void)
{
	return nw;
}
void Parser::GetWord(char dst[],int maxlen,int i)
{
	if(0<maxlen)
	{
		dst[0]=0;
		if(0<=i && i<nw)
		{
			int j;
			for(j=0; j<wLen[i] && j<maxlen-1; ++j)
			{
				dst[j]=str[wTop[i]+j];
			}
			dst[j]=0;
		}
	}
}

class Drawing
{
protected:
    int pointLen = 0;
    int *points;
    int *initpoints;
    
    int initx;
    int inity;
    
public:
    
    
    Drawing()
    {
        points = nullptr;
        initpoints = nullptr;
        

    }
    ~Drawing()
    {
        // std::cout << "in destructor\n";
        // std::cout << "points address : " << points << "\n";
        delete[] points;
            points = NULL;
        
        // std::cout << "delteed points\n";
        delete[] initpoints;
            initpoints = NULL;
        
        // std::cout << "delteed initpoints\n";
        
    }
    
    Drawing &operator=(const Drawing &incoming)
    {
        // if incoming is not defined, there is nothing to do
        if(incoming.pointLen == 0)
        {
            return *this; // return the class object itself. (without doing anything)
        }

        // TODO: try to understand what *this is

        // set pointLen
        pointLen = incoming.pointLen;

        // check if points and initpoints are allocated
        if(points != nullptr)
        {
            delete[] points;
            points = nullptr;
        }
        if(initpoints != nullptr)
        {
            delete[] initpoints;
            initpoints = nullptr;
        }
        points = new int[pointLen*7];
        initpoints = new int[pointLen*7];

        // copy points and initpoints
        for(int i=0;i<pointLen*7;++i)
        {
            points[i] = incoming.points[i];
            initpoints[i] = incoming.initpoints[i];
        }

        return *this;
    }

    
    
    void Draw() const;
    void BackToOrigin();
    void Zoom();
    void Reverse();
    
    const int* Load(const char fName[])
    {
        FILE *fp=fopen(fName,"r");
        if(nullptr!=fp)
        {
            char str[256];
            if(nullptr!=fgets(str,255,fp))
            {
                // clear();
                
                pointLen = atoi(str);
                std::cout << pointLen << std::endl;
                points=new int [pointLen*7];
                initpoints=new int [pointLen*7];
                int c=0;
                while(nullptr!=fgets(str,255,fp))
                {
                    Parser parser;  
                    if(2<=parser.Parse(str))
                    {
                        char x1[16],y1[16],x2[16],y2[16],R[16],G[16],B[16];
                        parser.GetWord(x1,16,0);
                        parser.GetWord(y1,16,1);
                        parser.GetWord(x2,16,2);
                        parser.GetWord(y2,16,3);
                        parser.GetWord(R,16,4);
                        parser.GetWord(G,16,5);
                        parser.GetWord(B,16,6);
                        points[c*7]   = atoi(x1);       // x1
                        points[c*7+1] = atoi(y1);       // y1
                        points[c*7+2] = atoi(x2);       // x2
                        points[c*7+3] = atoi(y2);       // y2
                        points[c*7+4] = atoi(R);        // R
                        points[c*7+5] = atoi(G);        // G
                        points[c*7+6] = atoi(B);        // B
                        ++c;
                    }
                }

                for(int i=0;i<pointLen*7;++i) initpoints[i] = points[i];
                return points;
                
            }
            fclose(fp);
        }
        else
        {
            printf("Could not open file.\n");
            
        }
        return 0;
    
    }

    void Move()
    {
        
        int lb,mb,rb,mx,my;
        int clickState = 0;
        
        auto key = FsInkey();
        auto mState = FsGetMouseEvent(lb,mb,rb,mx,my);

        if(FSMOUSEEVENT_LBUTTONDOWN==mState)
		{
            initx = mx;
            inity = my;
		}


        if(0!=lb)       // Pressing Left button down.
        {
                
            if(FSMOUSEEVENT_MOVE == mState)
            {
                
                for(int i=0; i<pointLen; ++i)
                {
                    points[i*7]   = (mx - initx) + initpoints[i*7];
                    points[i*7+1]   = -(my - inity) + initpoints[i*7+1];
                    points[i*7+2] = (mx - initx)  + initpoints[i*7+2];
                    points[i*7+3]   = -(my - inity) + initpoints[i*7+3];

                }
                
            }
            
        }
    }
};
void Drawing::BackToOrigin()
{
    for(int i=0;i<pointLen*7;++i) points[i] = initpoints[i];
}
void Drawing::Zoom()
{
    for(int i=0;i<pointLen;++i)
    {
        points[i*7] *= 1.1;
        points[i*7+1] *= 1.1;
        points[i*7+2] *= 1.1;
        points[i*7+3] *= 1.1;
    }
}
void Drawing::Reverse()
{
    if(points[1]> 0.1*initpoints[1])
    {
        for(int i=0;i<pointLen;++i)
        {
            points[i*7] *= 0.9;
            points[i*7+1] *= 0.9;
            points[i*7+2] *= 0.9;
            points[i*7+3] *= 0.9;
        }
    }
    
}

void Drawing::Draw() const
{
    
    for(int i=0; i<pointLen; ++i)
    {
        glColor3ub(points[i*7+4],points[i*7+5],points[i*7+6]);
        glBegin(GL_LINE_LOOP);
        glVertex2i(points[i*7],600 - points[i*7+1]);
        glVertex2i(points[i*7+2],600 - points[i*7+3]);
        glEnd();
    }
    glFlush();
}


char *MyFgets(char str[],int limit,FILE *fp)
{
    if(nullptr!=fgets(str,limit,fp))
    {
        int l=0;
        for(l=0; 0!=str[l]; ++l)
        {
        }
        for(l=l-1; 0<=l && ('\n'==str[l] || '\r'==str[l]); --l) 
        {
            str[l]=0; 
        }
        return str; 
    }
    return nullptr;
}
Drawing LoadFile(const char fn[])
{
    Drawing d; 
    if(0==d.Load(fn)) 
    {
        printf("Cannot read the file.\n");
        exit(1);
    }
    return d; 
}
int main(void)
{
    char fn[256];
    printf("Enter File Name>");
    MyFgets(fn,255,stdin);
    Drawing load=LoadFile(fn),drawing;
    drawing=load; // Force using operator=

    FsOpenWindow(16,16,800,600,1);
    for(;;)
    {
        FsPollDevice();
        auto key=FsInkey();
        if(FSKEY_ESC==key)
        {
            break; 
        }
        if(FSKEY_SPACE==key)
        {
            drawing.BackToOrigin();
        }
        if(FSKEY_Z==key)
        {
            drawing.Zoom();
        }
        if(FSKEY_M==key)
        {
            drawing.Reverse();
        }
        
        drawing.Move();
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 
        
        drawing.Draw();
        FsSwapBuffers();
        FsSleep(10);
    }
    return 0; 
}
