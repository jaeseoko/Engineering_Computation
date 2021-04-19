#include <stdio.h>
#include <time.h>

int main(void)
{
    const int res=40000;

    auto t0=time(nullptr);

    int M=0;
    const int N=res*res;
    for(int x=0; x<res; ++x)
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

    const double pi=2.0*2.0*(double)M/(double)N;
    printf("Estimated PI=%lf\n",pi);
    printf("Time Elapsed=%d\n",(int)(time(nullptr)-t0));

    return 0;
}
