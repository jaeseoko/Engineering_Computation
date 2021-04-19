#include <iostream>
#include <time.h>
#include <vector>
#include <stdlib.h>
using namespace std;

void Swap(vector<int> &a, vector<int> &b)
{
    auto c = a;
    a=b;
    b=c;
}
vector<vector<int>> Shuffle(vector<vector<int>> &cards)
{
    for(int i=0; i<cards.size(); ++i)
    {
        int j = rand()%cards.size();
        Swap(cards[i],cards[j]);
    }
    return cards;
}
//make 144 cards e.g) 1x1 , 1x2, ...
vector<vector<int>> GenerateCards()
{
    vector<vector<int>> cards{};
    for (int i = 1; i<13; i++)
    {
        for (int j = 1; j<13; j++)
        {
            cards.push_back({i,j,i*j});
        }
    }
    return cards;
}
int main()
{
    int input;
    int correct = 0; // ############### If not initizliaze int, number defined weird... why?????
    int wrong = 0;
    int percent;
    int mysol;
    int time_taken;
    
    // Get Cards && Get RandomSeed && Shuffle cards randomly
    auto cards = GenerateCards();
    srand(time(nullptr));
    cards = Shuffle(cards);
    
    cout << "Choose number of cards to play between(1 and 144): " << endl;
    cin >> input;
    while (input < 1 || input > 144)
    {
        cout <<"The number of cards should be between 1 and 144: " << endl;
        cin >> input;
    }
    time_t start,end;
    time(&start);
    for(int i=0; i<input; ++i)
    {
        int answer = cards[i][2];
        cout << cards[i][0] << "x" << cards[i][1] << "=>? \n";
        
        cin >> mysol;
        if (answer==mysol)
        {
            correct++; 
        }
    }
    time(&end);
    time_taken = int(end-start);
    percent = (correct*100.0)/(input);
    cout << "You answered " << input << " problems in " << time_taken << " seconds." << "\n";
    cout << "You answered " << correct << " problems correctly" << "("<< percent << "%)" << "\n";
}