#include <iostream>
#include <stdlib.h>
//#include <cstdlib>
//#include <ctime>
#include <chrono>
using std::cin;
using std::cout;
//using std::time;
using namespace std::chrono;


int main() {
  srand(time(0));
  int guess;
  int randomNumber;
  int answer = 1;
  for (int i = 0; i < 3; i++) {
    randomNumber = (rand() % 9) + 1;
    cout << randomNumber << "x";
    answer = randomNumber * answer;
  }
  cout << "=?>" << "\n";
  auto start = high_resolution_clock::now();
  cin >> guess;
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<seconds>(stop - start);
  if (guess == answer){
      cout << "Correct!" << " You spent " << duration.count() << " seconds to calculate." << "\n";
  }
  else {
      cout << "Wrong! Correct answer is " << answer << "." << "\n";
  }
}