#include <iostream>
#include <cmath>
using namespace std;



double Func(double x, double y)
{
    return sqrt(1- (x*x) - (y*y));
}

int main(void)
{
    double x0 = 0.0;
    double x1 = 1.0;
    double h = 0;
    cout << "Enter Integration Step > ";
    cin >> h;   

    double sum = 0.0;
    for (double y= 0; y < 1; y+=h) 
    {
        // cout << x << "\n";
        // for (double y= 0; y < sqrt(1-(x*x)); y+=h)
        for (double x=x0; x < x1; x+=h)
        {
            // cout <<"x^2 + y^2= "<< (x*x)+(y*y) << "\n";
            double z = Func(x,y);
            // cout << z << endl;
            sum+=z*h*h;
            // cout << sum << endl;
        }
    }
    sum = 2*sum;
    cout << "Double Integration = " << sum << "\n";
    return 0;
}
