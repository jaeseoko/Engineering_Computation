#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "player.h"
#include "obstacle.h"
#include "Enemy.h"
#include "Item.h"
using namespace std;


/*
Numbering in bitmap >>

     1 = shooting object(ball / pig) / player
     2 = Enemy
     3 = obstacle type 1
     4 = obstacle type 2
       
Initialize positions of corresponding objects based on sample map.
Create class objects with initialized positions and pass on to each class

    Initialize items (no.) obstacle with numbering, player, enemies, items.
*/

class Levels
{
private:
    int wid=100, hei=80;
    char *data;
public:
    Levels();
    ~Levels();
    void readAndSetMap(string fileName);
    void storePosition(void);
    void load(int lv);

    vector<int> playerPosition {};
    vector<vector<int>> enemyPosition {};
    vector<vector<int>> obstaclePosition {};
    
    Player createPlayer(void);    
    vector<Enemy> createEnemy(void);
    vector<Obstacle> createObstacle(void);
    vector<Item> createItems(void);

};

Levels::Levels()
{
std::cout << __FUNCTION__ << std::endl;    
    data = nullptr;
}

Levels::~Levels()
{
std::cout << __FUNCTION__ << std::endl;
    if(nullptr!=data)
    {
        delete[] data;
        data = nullptr;
    } 
    
    playerPosition.clear();
    enemyPosition.clear();
    obstaclePosition.clear();
}

void Levels::readAndSetMap(string fileName)
{
std::cout << __FUNCTION__ << std::endl;

    ifstream myfile(fileName);

    if(myfile.is_open())
    {
        data = new char[wid*hei];
        char x;
        int n = 0;
        while(myfile.get(x))
        {
            if(x!='\"' && x!='\n')
            {
                data[n] = x;
                n++;
            }
        }
        cout << "n : " << n << endl;
    }
    // CHECK IF MAP READ CORRECTLY

    // for(int i=0;i<80;++i)
    // {
    //     for(int j=0;j<100;++j)
    //     {
    //         cout << data[i*100+j];
    //     }
    //     cout << endl;
    // }    
}


void Levels::storePosition(void)
{
std::cout << __FUNCTION__ << std::endl;
    // Positions in pixel coord per the agreement with the team.
    for(int i = 0; i<wid*hei; ++i)
    {
        
        if('1'==data[i]) playerPosition.push_back(   10* i%wid),playerPosition.push_back(  10* (int)(i/wid));
        else if('2'==data[i]) enemyPosition.push_back({  10* (i%wid), 10* (int)(i/wid)});
        else if('3'==data[i]) obstaclePosition.push_back({10* (i%wid), 10* (int)(i/wid),1});
        else if('4'==data[i]) obstaclePosition.push_back({10* (i%wid), 10* (int)(i/wid),2});
        // else if('4'==data[i]) groundPosition.push_back({  10* i%wid, 10* i/wid});
        else
        {
        }
        /*
        .
        .
        .
        */
    }
}


Player Levels::createPlayer(void)
{
std::cout << __FUNCTION__ << std::endl;
    Player player;
    player.x = playerPosition[0];
    player.y = playerPosition[1];
    return player;
}


vector<Enemy> Levels::createEnemy(void)
{
std::cout << __FUNCTION__ << std::endl;
    
    vector<Enemy> enemies (enemyPosition.size());
    
    for(int i=0; i<enemies.size(); ++i)
    {
        enemies[i].x = enemyPosition[i][0];
        enemies[i].y = enemyPosition[i][1];
    }
    return enemies;
}

vector<Obstacle> Levels::createObstacle(void)
{
std::cout << __FUNCTION__ << std::endl;
    vector<Obstacle> obstacles(obstaclePosition.size());
    
    for(int i=0; i<obstacles.size(); ++i)
    {
        obstacles[i].x   = obstaclePosition[i][0];
        obstacles[i].y   = obstaclePosition[i][1];
        obstacles[i].num = obstaclePosition[i][2];
    }
    return obstacles;
}
vector<Item> Levels::createItems(void)
{
std::cout<<__FUNCTION__<<std::endl;    
    vector<Item> items(3);
    int iNum =1;
    for(Item i : items)
    {
        i.num = iNum;
        iNum ++;
    }
    return items;
}

void Levels::load(int lv)
{
    // Set Game map
    if(1==lv) readAndSetMap("map1.txt");
    else if(2==lv) readAndSetMap("map2.txt");
    else if(3==lv) readAndSetMap("map3.txt");
    else if(4==lv) readAndSetMap("map4.txt");
    else if(5==lv) readAndSetMap("map5.txt");
    else
    {
    }
    // Store each object position
    storePosition();

    // Initialize class objects and return them.

    Player player = createPlayer();
    vector<Obstacle> obstacles = createObstacle();
    vector<Enemy> enemies = createEnemy();
    vector<Item> items = createItems();
    
}