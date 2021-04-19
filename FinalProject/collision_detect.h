#include<stdio.h>
#include<vector>
#include<time.h>
#include<math.h>

#include "fssimplewindow.h"

const double DT=0.025;
const double constPI = 3.14159;
const double windowWidth = 800;
const double windowHeight = 600;
const double groundHeight = windowHeight-150.;
const double playerOriginX = 50;
const double playerOriginY = 50;


// for unit test
void drawCircle(double x, double y, double r,  int colorR, int colorG, int colorB)
{
    glColor3ub(colorR, colorG, colorB);
	glBegin(GL_TRIANGLE_FAN);

    double cx = (double) x;
    double cy = (double) y;

	for(int i=0; i<64; ++i)
	{
		double a=(double)i*2.0*constPI/64.0;
		double s=sin(a);
		double c=cos(a);
		glVertex2d(cx+c*r,cy+s*r);
	}
	glEnd();
}

class DummyItem
{
    public:
        double x, y, w, l, vx, vy, vxNext, vyNext;
        int dmg, state;
        int c;
        void move(void){
            //printf("%f, %f\n", x, y);
            x += vx*DT;
            y += vy*DT;
            //printf("%f, %f\n", x, y);

        }
        void draw(void){
            if(state==1){
                switch(c){
                case 1:
                    drawCircle(x, y, (l+w)/4., 255, 0, 0);
                    break;
                case 2:
                    drawCircle(x, y, (l+w)/4., 0, 255, 0);
                    break;
                case 3:
                    drawCircle(x, y, (l+w)/4., 0, 0, 255);
                    break;
                case 4:
                    drawCircle(x, y, (l+w)/4., 122, 122, 122);
                    break;
                }
            }
            
        };
};

// dummy object
class player: public DummyItem{public:player(double posX, double posY, double sizeX, double sizeY, double velX, double velY);};
class obstacle: public DummyItem{public:obstacle(double posX, double posY, double sizeX, double sizeY, double velX, double velY);};
class enemy: public DummyItem{public:enemy(double posX, double posY, double sizeX, double sizeY, double velX, double velY);};
class item: public DummyItem{public:int item_no;item(double posX, double posY, double sizeX, double sizeY, double velX, double velY, int no);};



player::player(double posX, double posY, double sizeX, double sizeY, double velX, double velY)
{
    x = posX;
    y = posY;
    w = sizeX;
    l = sizeY;
    vx = velX;
    vy = velY;
    vxNext = velX;
    vyNext = velY;
    c=1;
    dmg = 0;
    state=1;
}

obstacle::obstacle(double posX, double posY, double sizeX, double sizeY, double velX, double velY)
{
    x = posX;
    y = posY;
    w = sizeX;
    l = sizeY;
    vx = velX;
    vy = velY;
    vxNext = velX;
    vyNext = velY;
    c=2;
    dmg = 0;
    state=1;

}

enemy::enemy(double posX, double posY, double sizeX, double sizeY, double velX, double velY)
{
    x = posX;
    y = posY;
    w = sizeX;
    l = sizeY;
    vx = velX;
    vy = velY;
    vxNext = velX;
    vyNext = velY;
    c=3;
    dmg = 0;
    state=1;

}

item::item(double posX, double posY, double sizeX, double sizeY, double velX, double velY, int no)
{
    x = posX;
    y = posY;
    w = sizeX;
    l = sizeY;
    vx = velX;
    vy = velY;
    vxNext = velX;
    vyNext = velY;
    c=4;
    dmg = 0;
    state = 1;
    item_no = no;
}

double Distance(double x1,double y1,double x2,double y2)
{
	double dx=x2-x1;
	double dy=y2-y1;
	return sqrt(dx*dx+dy*dy);
}

bool valueInRange(int value, int min, int max)
{ return (value >= min) && (value <= max); }

// check if one point is in another rectangle
bool checkPointOverlap(double x0, double y0, double x_min, double x_max, double y_min, double y_max)
{
    return(x0>=x_min && x0<=x_max && y0>=y_min && y0<=y_max);                  
}

// check if two rectangles have intersect area
bool checkRectOverlap(double rectAx1, double rectAx2, double rectAy1, double rectAy2,
                      double rectBx1, double rectBx2, double rectBy1, double rectBy2)
{   return (rectAx1 < rectBx2 && rectAx2 > rectBx1 && rectAy1 > rectBy2 && rectAy2 < rectBy1);} 

// apply elastic collision to two mass points
void calcCollision(double x1, double x2, double y1, double y2,
                    double mass1, double mass2, 
                    double &vx1, double &vx2, double &vy1, double &vy2,
                    double coef_rest)
{
    double dotProd = (vx1 - vx2) * (x1 - x2) + (vy1 - vy2) * (y1 - y2);
    double dist2 = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
    if(dotProd<0.0){
        double vx1_new = vx1 - (1.+coef_rest) * mass2/(mass1+mass2) * dotProd/dist2 * (x1-x2);
        double vy1_new = vy1 - (1.+coef_rest) * mass2/(mass1+mass2) * dotProd/dist2 * (y1-y2);

        double vx2_new = vx2 - (1.+coef_rest) * mass1/(mass1+mass2) * dotProd/dist2 * (x2-x1);
        double vy2_new = vy2 - (1.+coef_rest) * mass1/(mass1+mass2) * dotProd/dist2 * (y2-y1);
        vx1 = vx1_new;
        vy1 = vy1_new;
        vx2 = vx2_new;
        vy2 = vy2_new;
    }   
}

template <typename T1, typename T2>
bool collideWithSingle(T1 &obj1, T2 &obj2, double coef_rest, bool canDmg){
    // check collision
    double r1 = std::max(obj1.w, obj1.l)/2.;
    double r2 = std::max(obj2.w, obj2.l)/2.;
    double r = Distance(obj1.x, obj1.y, obj2.x, obj2.y);
    bool isCol = r < (r1+r2);
    // apply collision effect, modify velocity
    if(isCol)
    {
        double vx1  = obj1.vx;
        double vy1  = obj1.vx;
        double vx2  = obj2.vx;
        double vy2  = obj2.vy;

        calcCollision(obj1.x, obj2.x, obj1.y,obj2.y, 1.0, 1.0, vx1, vx2, vy1, vy2, coef_rest); 
        obj1.vxNext = vx1;
        obj1.vyNext = vy1;
        obj2.vxNext = vx2;
        obj2.vyNext = vy2;
        if(canDmg){
            obj1.dmg++;
            obj2.dmg++;
        }    
    }
    return isCol;
}

// loop through a vector of ojects, starting from startIdx, check if collision happen bewteen obj1 and obj in objs
// coefficient of collision. coef_rest=0.0: perfectly inelastic; coef_rest=1.0: prefectly elastic
// canDmg if the object collision will cause damage (ideally, only player can cause damage)
template <typename T1, typename T2>
bool collideWithMultiple(T1 &obj1, std::vector<T2> &objs, uint startIdx, double coef_rest, bool canDmg)
{
    bool hasCol = false;
    for (uint j=startIdx; j<objs.size(); ++j)
    {
        T2 &obj2 = objs[j];
        bool isCol = collideWithSingle(obj1, obj2, coef_rest, canDmg);
        if(!hasCol&&isCol){hasCol=true;}

    }
    return hasCol;
}

// return false if object out of boundary, so it will be erase
// will increase the dmg of object by one
template<typename T>
bool applyCollision(T &obj){
    if((!valueInRange(obj.x, 0.-obj.w/2., windowWidth+obj.w/2.))||
        obj.y+obj.l/2.<0)// out of boundary
    {
        return false;
    }
    if(obj.y+obj.l/2.>groundHeight){
        obj.vyNext=-0.2*obj.vyNext;
    }
    double eps=0.001;

    // collision happen
    if(abs(obj.vxNext-obj.vx)>eps||abs(obj.vyNext-obj.vy)>eps)
    {
        obj.x += (obj.vxNext-obj.vx)*DT; //+ obj.w/4. * (obj.vxNext)/sqrt(obj.vxNext*obj.vxNext);
        obj.y += (obj.vyNext-obj.vy)*DT; //+ obj.l/4. * (obj.vyNext)/sqrt(obj.vyNext*obj.vyNext);
        obj.vx = obj.vxNext;
        obj.vy = obj.vyNext;
    }
    return true;   
}



bool checkCollision(player &p, std::vector<obstacle> &obstacles, std::vector<enemy> &enemies, std::vector<item> &items, int &itemHit) {
   
   bool hasColAll = false;
   bool hasCol = false;
   // obstacle collision with each other
   for (uint i=0; i<obstacles.size(); ++i)
   {
        obstacle &obs1 = obstacles[i];
        hasCol = collideWithMultiple<obstacle, obstacle>(obs1, obstacles, i+1, 0.9, false);
        if(!hasColAll && hasCol){hasColAll=true;}
   } 

   // obstacle collision with enemies
   for (uint i=0; i<obstacles.size(); ++i)
   {
        obstacle &obs1 = obstacles[i];
        hasCol = collideWithMultiple<obstacle, enemy>(obs1, enemies, 0, 0.9, false);
        if(!hasColAll && hasCol){hasColAll=true;}
   } 
   
   // obstacle collision with player
   hasCol = collideWithMultiple<player, obstacle>(p, obstacles, 0, 0.9, false);
   if(!hasColAll && hasCol){hasColAll=true;}


   // enemy collision with each other
   for (uint i=0; i<enemies.size(); ++i)
   {
        enemy &enm1 = enemies[i];
        hasCol = collideWithMultiple<enemy, enemy>(enm1, enemies, i+1, 0.9, false);
        if(!hasColAll && hasCol){hasColAll=true;}

   }

    // enemy collision with player
    hasCol = collideWithMultiple<player, enemy>(p, enemies, 0, 0.9, true);
    if(!hasColAll && hasCol){hasColAll=true;}

    // check player hit item
    int hitItemType = 0;
    for (uint i=0; i<items.size(); ++i)
    {
        item &itm = items[i];
        hasCol = collideWithSingle<player, item>(p, itm, 0.9, false);
        if(hasCol && hitItemType==0){hitItemType=itm.item_no;}
        if(!hasColAll && hasCol){hasColAll=true;}
    }
    itemHit = hitItemType;

    
    for(unsigned idx=0; idx<obstacles.size(); ++idx)
    {
        auto &o = obstacles[idx];
        if(!applyCollision(o)){obstacles.erase(obstacles.begin()+idx);}
    }

    for(unsigned idx=0; idx<enemies.size(); ++idx)
    {
        auto &e = enemies[idx];
        if(!applyCollision(e)){enemies.erase(enemies.begin()+idx);}
    }

    if(!applyCollision(p)){
        p.x = playerOriginX;
        p.y = playerOriginY;
        p.dmg = 0;
        p.state = 0;
        p.vx = 0.;
        p.vy = 0.;
    };
    return hasColAll;
}
