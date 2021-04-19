#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
using namespace std;

class Drawing
{
// protected:

public:
    int * str;
    int n; // number of lines
    Drawing();       // Default Constructor
	Drawing(const Drawing &incoming); // Constructor w/ parameters
	Drawing &operator=(const Drawing &incoming)
    {
        n = incoming.n;
        str = incoming.str;    
    }
	// ~Drawing();
    void fgets(const string &filename)
    {
        ifstream mfile(filename.c_str());
        mfile >> n;
        
        str = new int[n*7];
        int x;
        int ind = 0;
        while(mfile >> x)
        {
            str[ind] = x;
            ind++;
        }
    }
    void print(void) const
    {
        for(int i=0; i<n; ++i)
        {
            for (int j=0; j<7; ++j)
            {
                cout << str[7*i+j] << " ";
            }
            cout << endl;
        }
    
    }
    void push_back(int a)
    {
        if(str== nullptr)
        {
            str = new int[1];
            str[0] = a;
            n=1;
            return;
        }
        int * temp = new int[n+1];
        
        for(int i=0; i<7*n; ++i)
        {
            temp[i] = str[i];
        }
        temp[n] = a;
        delete[] str;
        str=nullptr;
        str = temp;
        n++;
    }
    string data(void) const
    {
        string s = "";
        for(int i=0; i<n; ++i)
        {
            s += to_string(str[i]);
        }
        return s;
    }
};

Drawing::Drawing()
{
    n = 0;
    str = nullptr;
}
Drawing::Drawing(const Drawing &incoming)
{
    n = incoming.n;
    str = incoming.str;    
}

// Drawing::~Drawing()
// {
//     delete [] str;
// }
int main(void)
{
    Drawing s1,s2;

	s1.push_back(1);
	s1.push_back(2);
	s1.push_back(3);

	s2=s1;

	std::cout << "s2=" << s2.data() << std::endl;

	Drawing s3(s2);
	std::cout << "s3=" << s3.data() << std::endl;
}