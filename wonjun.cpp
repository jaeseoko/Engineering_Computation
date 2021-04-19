#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class Drawing{
public:
    int n;
    char* str;
    Drawing()
    {
        n = 0;
        str = NULL;
    }    

    Drawing& operator=(const Drawing& other)
    {
        if(n > 0)
        {
        delete[] str;
        str = NULL;
        }
        n = other.n;
        str = new char[n];
        for(int i=0;i<n;++i) 
        {
            str[i] = other.str[i];
        }
        return *this;
    }

    void push_back(const char c)
    {
        if(n == 0)
        {
            str = new char[1];
            str[0] = c;
            n = 1;
            return;
        }
        char* tmp = new char[n+1];
        for(int i=0;i<n;++i)
        {
            tmp[i] = str[i];
        }
        tmp[n] = c;
        delete[] str;
        str = tmp;
        ++n;
    }

    char* data() const
    {
        return str;
    }
};

int main()
{
    Drawing a,b;
    a.push_back('C');
    a.push_back('M');
    a.push_back('U');

    b = a;

    cout << "a : " << a.data() << "\n";
    cout << "b : " << b.data() << "\n";
}