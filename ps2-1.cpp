#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
using namespace std;


int GetRandomNumber()
{
    srand(time(nullptr));
    return rand()%6;
}

//const string foods[6] = {"hamburger", "pizza", "sub", "hot dog", "doughnut", "pancake"};

/*void Swap(string &a, string &b)
{
    auto c = a;
    a=b;
    b=c;
}*/

/*string Shuffle(int len, string foods[6])
{
    for(int i=0; i<len; ++i)
    {
        int j = rand()%len;
        Swap(foods[i],foods[j]);
    }
    return foods[6];
}*/

int main()
{
    string phrase = "You should have a";
    int RandN = GetRandomNumber();
    cout << "Press enter to continue . . ." << "\n";
    cin.ignore();
    switch(RandN)
    {
        case 0: cout << phrase << " hamburger" << "\n";
        break;
        case 1: cout << phrase << " pizza" << "\n";
        break;
        case 2: cout << phrase << " sub" << "\n";
        break;
        case 3: cout << phrase << " hot dog" << "\n";
        break;
        case 4: cout << phrase << " doughnut" << "\n";
        break;
        case 5: cout << phrase << " pancake" << "\n"; 
    }
}
