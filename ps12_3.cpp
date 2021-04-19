#include <stdio.h>
#include <thread>
#include <time.h>

/* 

The following program estimates the value of π by sampling points over a rectangular lattice. 
Similar idea as the Monte-Carlo, but in this problem we don’t use random sample points.
Modify the program so that it calculates the value in parallel
using std::thread. Your program should use eight threads.

Hint: For thread K (0<=K<4), the range of x should be:
        res*K/4<=x<res*(K+1)/4

*/

class ThreadFunc
{
protected:
    
public:
    int M;
    int x0,x1,y0,y1;
    static void Entry(ThreadFunc *thisPtr, const int res);
};
/* static */ void ThreadFunc::Entry(ThreadFunc *thisPtr,const int res)
{
    auto x0 = thisPtr->x0;
    auto x1 = thisPtr->x1;
    
    int M = 0;
    for(int x=x0; x<x1; ++x)
    {
        for(int y=0; y<res; ++y)
        {
            const double xx=(double)x/(double)res;
            const double yy=(double)y/(double)res;

            if(xx*xx+yy*yy<=1.0)
            {
                M++;
            }
        }
    }
    thisPtr->M = M;
}



int main(void)
{
    const int res=40000;

    auto t0=time(nullptr);

    int M=0;
    const int N=res*res;
    
    
    const int tNum = 8;                     // Set Number of threads to use.
    ThreadFunc params[tNum];

    // Set range of x for each thread
    for(int i = 0; i<tNum; ++i)
    {
        params[i].x0 = res*i/tNum;
        params[i].x1 = res*(i+1)/tNum;
    }
    std::thread th0(ThreadFunc::Entry,&params[0],res);
    std::thread th1(ThreadFunc::Entry,&params[1],res);
    std::thread th2(ThreadFunc::Entry,&params[2],res);
    std::thread th3(ThreadFunc::Entry,&params[3],res);
    std::thread th4(ThreadFunc::Entry,&params[4],res);
    std::thread th5(ThreadFunc::Entry,&params[5],res);
    std::thread th6(ThreadFunc::Entry,&params[6],res);
    std::thread th7(ThreadFunc::Entry,&params[7],res);

    th0.join(),th1.join(),th2.join(),th3.join();
    th4.join(),th5.join(),th6.join(),th7.join();

    for(int i=0; i<tNum; ++i)
    {
        M+=params[i].M;
    }

    const double pi=2.0*2.0*(double)M/(double)N;
    printf("Estimated PI=%lf\n",pi);
    printf("Time Elapsed=%d\n",(int)(time(nullptr)-t0));

    return 0;
}
