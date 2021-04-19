#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Score
{
public:
    string pins = "1234567890- /Xx";
    int ScoreFromFrame(vector<int> nums,int frameNo);
    vector<int> convertToNum(string score);
};

vector<int> Score::convertToNum(string score)
{
    vector<int> num {};
    for(int s=0; s<21; ++s)
    {
        for(int i =0; i<pins.length(); ++i)
        {
            if(score[s]==pins[i])
            {
                if(i<9) num.push_back(i+1);
                else if(i==9 ||i==10 || i==11) num.push_back(0);
                else if(i==12) num.push_back(10-num[s-1]);
                else num.push_back(10);
            }
        }
    }
    return num;
}
int Score::ScoreFromFrame(vector<int> nums, int frameNo)
{
    int myScore = 0;
    int A=2*frameNo-2,B=2*frameNo-1,C = 2*frameNo,D=2*frameNo+1,E =2*frameNo+2;
    if(frameNo<9)
    {
        if(nums[A]==10&& nums[C]==10) myScore = 20+nums[E];     // 2 or 3 strikes
        else if(nums[A]==10) myScore = 10 + nums[C]+nums[D];    // strike
        else if(nums[A]+nums[B]==10) myScore = 10 + nums[C];    // spare
        else myScore = nums[A]+nums[B];                         // else
    }
    else if(frameNo==9)
    {
        if(nums[A]==10&& nums[C]==10) myScore = 20+nums[D];
        else if(nums[A]==10) myScore = 10 + nums[C]+nums[D];
        else if(nums[A]+nums[B]==10) myScore = 10 + nums[C];
        else myScore = nums[A]+nums[B];
    }
    else if(frameNo==10) myScore= nums[A]+nums[B]+nums[C];
    return myScore;
}


int main(void)
{   
    Score score;
    string board;
    int iter = 0;
    int totalScore = 0;
    // cout << "Enter Score board\n";
    // cout << "> ";
    getline(cin,board);
    for(auto &b: board)
    {
        for(auto &p: score.pins)
        {
            if(p==b) iter++;
        }
        if(iter!=1)
        {
            cout << "Wrong Character\n";
            return 0;
        }
        else iter = 0;
    }
    if(board.length()<21) 
    {
        cout << "Too Short\n";
        return 0;
    }
    
    
    vector<int> scores = score.convertToNum(board);
    for(int frameNo=1; frameNo<11; ++frameNo)
    {
        totalScore += score.ScoreFromFrame(scores,frameNo);
    }
    cout << totalScore << endl;

    return 0;
}