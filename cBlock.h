#pragma once
#include "cBicho.h"
#include "cPlayer.h"
#include "cMonster.h"

#define STATE_DESTROYED 0
#define STATE_BROWN 1
#define STATE_GREEN 2

class cPlayer;
class cMonster;

class cBlock :
	public cBicho
{
public:
	cBlock(void);
	virtual ~cBlock(void);

	virtual bool Appears();
	virtual bool isCollisionable();
	virtual void Destroy();

	virtual void Draw(int tex_id);
	virtual void Logic(cPlayer &player, int &money, bool &ring, int &lifes, std::vector<cMonster*> &monsters);
};

