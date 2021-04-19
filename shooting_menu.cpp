#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "fssimplewindow.h"
#include "yssimplesound.h"
#include "ysglfontdata.h"



const int nTargets=5;
const int nMissiles=5;

YsSoundPlayer player;



bool CheckCollision(int mx,int my,int tx,int ty,int tw,int th)
{
	int dx=mx-tx;
	int dy=my-ty;
	if(0<=dx && dx<tw && 0<=dy && dy<th)
	{
		return true;
	}
	return false;
}

const int nParticlePerExplosion=100;
class Explosion
{
public:
	int state,counter;
	int xy[nParticlePerExplosion*2];
	int vxvy[nParticlePerExplosion*2];
	void Initialize(void);
	bool Begin(int x,int y);
	void Move(void);
	void Draw(void);
	void Disappear(void);
};
void Explosion::Initialize(void)
{
	state=0;
}
bool Explosion::Begin(int x,int y)
{
	if(0==state)
	{
		state=1;
		counter=0;
		for(int i=0; i<nParticlePerExplosion; ++i)
		{
			xy  [i*2  ]=x;
			xy  [i*2+1]=y;
			vxvy[i*2  ]=rand()%11-5;
			vxvy[i*2+1]=rand()%11-5;
		}
		return true;
	}
	return false;
}
void Explosion::Move(void)
{
	for(int i=0; i<nParticlePerExplosion; ++i)
	{
		xy[i*2  ]+=vxvy[i*2  ];
		xy[i*2+1]+=vxvy[i*2+1];
	}

	++counter;
	if(50<counter)
	{
		Disappear();
	}
}
void Explosion::Draw(void)
{
	glPointSize(5);
	glColor3ub(255,0,0);
	glBegin(GL_POINTS);
	for(int i=0; i<nParticlePerExplosion; ++i)
	{
		glVertex2i(xy[i*2  ],xy[i*2+1]);
	}
	glEnd();
}
void Explosion::Disappear(void)
{
	state=0;
}

class Target
{
public:
	int x,y,state,w,h,v;
	bool IsAlive(void);
	void Initialize(void);
	void Move(void);
	void Draw(void);
	void Explode(void);
};
bool Target::IsAlive(void)
{
	return 0!=state;
}
void Target::Initialize(void)
{
	x=0;
	y=50+rand()%50;
	w=80+rand()%40;
	h=20;
	v=15+rand()%10;
	state=1;
}
void Target::Move(void)
{
	if(0!=state)
	{
		x+=v;
		if(800<=x)
		{
			x=0;
		}
	}
}
void Target::Draw(void)
{
	glColor3ub(0,0,255);
	glBegin(GL_QUADS);
	glVertex2i(x  ,y);
	glVertex2i(x+w,y);
	glVertex2i(x+w,y+h);
	glVertex2i(x  ,y+h);
	glEnd();
}

class Missile
{
public:
	int x,y,state;
	bool IsFlying(void);
	void Initialize(void);
	bool CheckCollision(Target t);
	bool GoneOutOfWindow(void);
	bool TryShoot(int x0,int y0);
	void Move(void);
	void Draw(void);
};
bool Missile::IsFlying(void)
{
	return 0!=state;
}
void Missile::Initialize(void)
{
	state=0;
}
bool Missile::CheckCollision(Target t)
{
	return ::CheckCollision(x,y,t.x,t.y,t.w,t.h);
}
bool Missile::GoneOutOfWindow(void)
{
	return y<0;
}
void Missile::Move(void)
{
	if(0!=state)
	{
		y-=10;
		if(true==GoneOutOfWindow())
		{
			state=0;
		}
	}
}
bool Missile::TryShoot(int x0,int y0)
{
	if(0==state)
	{
		state=1;
		x=x0;
		y=y0;
		return true;
	}
	return false;
}
void Missile::Draw(void)
{
	glColor3ub(255,0,0);
	glBegin(GL_QUADS);
	glVertex2i(x-2,y);
	glVertex2i(x+2,y);
	glVertex2i(x+2,y+9);
	glVertex2i(x-2,y+9);
	glEnd();
}

class SpaceShip
{
public:
	int x,y;
	void Initialize(void);
	void Draw(void);
};
void SpaceShip::Initialize(void)
{
	x=400;
	y=550;
}
void SpaceShip::Draw(void)
{
	glColor3ub(0,255,0);
	glBegin(GL_QUADS);

	glVertex2i(x-15,y);
	glVertex2i(x-15,y-9);
	glVertex2i(x+14,y-9);
	glVertex2i(x+14,y);

	glVertex2i(x-5,y-19);
	glVertex2i(x+5,y-19);
	glVertex2i(x+5,y);
	glVertex2i(x-5,y);

	glEnd();
}

class GameMenu
{
public:
	int key;
	void Initialize(void);
	void RunOneStep(void);
	void Draw(void);
};

void GameMenu::Initialize(void)
{
	key=0;
}

void GameMenu::RunOneStep(void)
{
	FsPollDevice();
	key=FsInkey();
}

void GameMenu::Draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glColor3ub(0,0,0);
	glRasterPos2i(64,64);
	YsGlDrawFontBitmap16x20("S.....Start");
	glRasterPos2i(64,84);
	YsGlDrawFontBitmap16x20("ESC...Quit");
	FsSwapBuffers();
}

class GameMain
{
public:
	bool gameEnded;
	int nUsed=0;
	SpaceShip ship;
	Missile missile[nMissiles];
	Target target[nTargets];
	Explosion exp[nTargets];

	void Initialize(void);
	bool GameEnded(void);
	void RunOneStep(class ShootingGame &program);
	void Draw(void);
};

class ShootingGame
{
public:
	YsSoundPlayer::SoundData bangSE,missileSE;
	void Run(void);
};

void GameMain::Initialize(void)
{
	srand(time(nullptr));

	gameEnded=false;

	ship.Initialize();
	for(auto &t : target)
	{
		t.Initialize();
	}
	for(auto &m : missile)
	{
		m.Initialize();
	}
	for(auto &e : exp)
	{
		e.Initialize();
	}
}
bool GameMain::GameEnded(void)
{
	return gameEnded;
}
void GameMain::RunOneStep(class ShootingGame &program)
{
	player.KeepPlaying();

	FsPollDevice();
	auto key=FsInkey();
	if(FSKEY_ESC==key)
	{
		gameEnded=true;
		return;
	}

	// Moving part >>
	switch(key)
	{
	case FSKEY_SPACE:
		for(auto &m : missile)
		{
			if(true==m.TryShoot(ship.x,ship.y))
			{
				player.PlayOneShot(program.missileSE);
				++nUsed;
				break;
			}
		}
		break;
	}

	if(0!=FsGetKeyState(FSKEY_LEFT))
	{
		ship.x-=10;
	}
	if(0!=FsGetKeyState(FSKEY_RIGHT))
	{
		ship.x+=10;
	}

	for(auto &m : missile)
	{
		m.Move();
	}
	for(auto &t : target)
	{
		t.Move();
	}
	for(auto &e : exp)
	{
		e.Move();
	}
	for(auto &m : missile)
	{
		if(m.IsFlying())
		{
			bool shotDown=false;
			int nTargetsAlive=0;
			for(auto &t : target)
			{
				if(true==t.IsAlive() && true==m.IsFlying() && true==m.CheckCollision(t))
				{
					player.PlayOneShot(program.bangSE);
					for(auto &e : exp)
					{
						if(true==e.Begin(m.x,m.y))
						{
							break;
						}
					}
					m.state=0;
					t.state=0;
					shotDown=true;
				}
				nTargetsAlive+=t.state;
			}
			if(true==shotDown && 0==nTargetsAlive)
			{
				printf("Hit!\n");
				printf("You used %d missiles to shoot all down.\n",nUsed);
			}
		}
	}
	// Moving part <<

	FsSleep(10);
}
void GameMain::Draw(void)
{
	// Drawing part >>
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	ship.Draw();
	for(auto &m : missile)
	{
		if(m.IsFlying())
		{
			m.Draw();
		}
	}
	for(auto &t : target)
	{
		if(t.IsAlive())
		{
			t.Draw();
		}
	}
	for(auto &e : exp)
	{
		if(0!=e.state)
		{
			e.Draw();
		}
	}
	glFlush();
	FsSwapBuffers();
	// Drawing part <<
}

void ShootingGame::Run(void)
{
	/*
	  In Windows/Linux: Put .WAV in the same directory as .EXE
	  In macOS: Put .WAV in ???.app/Contents/Resources
	*/
	if(YSOK!=bangSE.LoadWav("bang.wav") ||
	   YSOK!=missileSE.LoadWav("missile.wav"))
	{
		printf("Load error.\n");
		return;
	}

	int progState=0; // 0:Menu  1:Game

	GameMenu menu;
	GameMain main;
	menu.Initialize();
	main.Initialize();
	for(;;)
	{
		FsPollDevice();

		// Moving
		if(0==progState)
		{
			menu.RunOneStep();
			if(FSKEY_S==menu.key)
			{
				main.Initialize();
				progState=1;
			}
			else if(FSKEY_ESC==menu.key)
			{
				break;
			}
		}
		else // if(1==progState)
		{
			main.RunOneStep(*this);
			if(true==main.GameEnded())
			{
				menu.Initialize();
				progState=0;
			}
		}

		// Drawing
		if(0==progState)
		{
			menu.Draw();
		}
		else
		{
			main.Draw();
		}
	}
}


int main(void)
{
	FsOpenWindow(0,0,800,600,1);
	FsChangeToProgramDir();
	player.Start();

	ShootingGame game;
	game.Run();

	player.End();
	return 0;
}
