#pragma once
#include "cBicho.h"
#include "cPlayer.h"
#include "cMonster.h"


#define BMON  1
#define SMON  2
#define RING  3
#define LIFE  4
#define SKULL 5
#define QUEST 6
#define STAR  7
#define SKULL_P	8
#define R_BROWN 9
#define R_GREEN 10

class cPlayer;
class cMonster;

class cBlock :
	public cBicho
{
public:
	cBlock(void);
	virtual ~cBlock(void);

	void SetTreasure(int treas);
	int GetTreasure();

	virtual bool Appears();
	virtual bool isCollisionable();
	virtual void Destroy();

	virtual void SetState(int st);

	virtual void Draw(int tex_id);
	virtual void Logic(cPlayer &player, int &money, bool &ring, int &lifes, std::vector<cMonster*> &monsters);

protected:
	bool collisionable;
	bool appears;
	static int rings_appeared;
	bool poisoned;
	bool touching;
	bool ghost_appears;
	int treasure;
};

