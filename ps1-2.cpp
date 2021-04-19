#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;
int main()
{
   int randomNumber, guess; 
   int answer = 1; 
   srand(time(nullptr));
   time_t start, end;
   for (int i = 0; i < 3; i++){
       randomNumber = (rand()%9) + 1;
       answer = randomNumber * answer;
       cout << randomNumber << "x";
   }
   cout << "=?>" << endl;
   time(&start);
   cin >> guess;
   time(&end);
   int time_taken = int(end-start);
   if (guess == answer){
       cout << "Correct! You spent " << time_taken << " seconds to calculate." <<"\n";
   }
   else {
       cout << "Wrong! Correct answer is " << answer << "." << "\n";
   }
   return 0;
}