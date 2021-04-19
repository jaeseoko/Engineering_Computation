#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;


// Define your own Drawing class.
class Reading
{
protected:
    char *str;
public:
    Reading();
    Reading(const Reading &incoming);
    Reading &operator=(const Reading &incoming);
    ~Reading();
    void cleanUp(void);
    int size(void) const;
    void push_back(char c);
    void pop_back(void);
    const char *data(void) const;
    const char *c_str(void) const;

    const char *fgets(FILE *fp);

    void copy_from(const Reading &incoming);
    void set(const char incoming[]);
};

Reading::Reading()
{
    str=nullptr;
}
Reading::~Reading()
{
    cleanUp();
}
void Reading::cleanUp(void)
{
    delete [] str;
    str = nullptr;
}
int Reading::size(void) const
{
    if(nullptr==str)
    {
        return 0;
    }
    int i;
    for(i=0; 0!=str[i]; ++i)
    {

    }
    return i;
}
void Reading::push_back(char c)
{
    auto s=size();
    // Allocate
    char *newStr = new char [s+2];
    // Copy
    for(int i=0; i<s; ++i)
    {
        newStr[i]=str[i];
    }
    // Append
    newStr[s]=c; // Add new char c
    newStr[s+1]=0; // Add terminator at the end

    // Clear
    cleanUp();

    // Repoint: original string is updated new
    str=newStr;
}

void Reading::pop_back(void)
{
    auto s=size();
    if(0<s)
    {
        // str[s]==0
        str[s-1]=0;

        // Allocate
        char *newStr=new char [s];

        // Copy
        for (int i=0; i<s; ++i)
        {
            newStr[i]=str[i];
        }

        // Clear
        cleanUp();
        // Repoint
        str = newStr;
    }
}

const char *Reading::data(void) const
{
    return (nullptr==str ? "" : str);
}
const char *Reading::c_str(void) const
{
    return (nullptr==str ? "" : str);
}


const char *Reading::fgets(FILE *fp)
{
    const int bufLen = 16;
    char buf[16];
    bool filled = false;

    this->cleanUp();
    
    while(nullptr!=::fgets(buf,15,fp))
    {
        filled = true;
        for(int i=0; 0!= buf[i]; ++i)
        {
            if(buf[i]=='\n' || buf[i]=='\r')
            {
                goto EXIT;
            }
            this->push_back(buf[i]);
        }
    }
EXIT:
    if(true==filled)
    {
        return this->data();
    }
    return nullptr;
}
Reading::Reading(const Reading &incoming)
{
    str=nullptr;
    copy_from(incoming);
}
Reading &Reading::operator=(const Reading &incoming)
{
    copy_from(incoming);
    return *this;
}
void Reading::copy_from(const Reading &incoming)
{
    set(incoming.c_str());
}
void Reading::set(const char incoming[])
{
    if(incoming!=str)
    {
        cleanUp();
        if(nullptr!=incoming)
        {
            for(int i=0; 0!=incoming[i]; ++i)
            {
                push_back(incoming[i]);
            }
        }
    }
}

class Drawing
{
protected:
    const char *load;
    const char *LoadFile;
public:
    Reading str;
    const char *Load(const char fn[]);
    void Draw(void) const;
    
    
};


const char* Drawing::Load(const char fn[])
{
    auto plot = str.fgets(*fn)
    int len = plot.data()[0];
    
    for(int i=0; i<len*7; ++i)
    {
        cout << plot.data()[i] <<endl;
    }
}
void Drawing::Draw(void) const
{
    
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
    
    return 0; 
}