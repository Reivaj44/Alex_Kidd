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

#define NO_RING 0
#define HV_RING 1
#define AP_RING	2

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
	virtual void Logic(cPlayer player, int &money, int &ring, int &lifes);

protected:
	bool collisionable;
	bool appears;
	bool ring_appeared;
	bool poisoned;
	int treasure;
};

