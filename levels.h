#ifndef LEVELS_H
#define LEVELS_H


#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "player.h"
#include "Item.h"
#include "Enemy.h"
#include "obstacle.h"



using namespace std;

class Levels
{
private:
    int wid = 100, hei = 100;
    char *data;
public:
    Levels();
    ~Levels();
    void SetBitMap(int mapWid,int mapHei,char map[]);
    // void SetObjPosition(int x,int y,char c);
    void StorePosition(void);
    void load(int v);

    
    vector<int> playerPosition {};
    vector<vector<int>> enemyPosition {};
    vector<vector<int>> obstaclePosition {};

    Player createPlayer(void);
    
    vector<Enemy> createEnemy(void);
    vector<Obstacle> createObstacle(void);
    vector<Item> createItems(void);
    
};




#endif