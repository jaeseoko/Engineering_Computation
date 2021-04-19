#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "player.h"

#include "obstacle.h"
#include "Enemy.h"
#include "levels.h"
using namespace std;



int main(void)
{

    Levels level;
    level.load(1);
    Player player = level.createPlayer();
    vector<Obstacle> obstacles = level.createObstacle();
    vector<Enemy> enemies = level.createEnemy();
    vector<Item> itmes = level.createItems();
    for(Enemy e: enemies)
    {
        cout << "enemy position x,y =\n";
        cout << e.x << "," << e.y << endl;
    }
    for(Obstacle o: obstacles)
    {
        cout << "obstacle position x,y AND numbering: \n";
        cout << o.x << "," << o.y << "," <<o.num << endl;
    }
    
    
    return 0;
}