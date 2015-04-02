#pragma once

#include "cBlock.h"
#include "cPlayer.h"

#define STATE_BMON  1
#define STATE_SMON  2
#define STATE_RING  3
#define STATE_LIFE	4
#define STATE_SKULL 5
#define STATE_QUEST 6
#define STATE_STAR  7
#define STATE_SKULL_P	8



class cBox :
	public cBlock
{
public:
	cBox(void);
	virtual ~cBox(void);

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

