#pragma once
#include "cBicho.h"
#include "cPlayer.h"
#include "cBlock.h"
#include <vector>

#define STATE_LEFT		0
#define STATE_RIGHT		1
#define STATE_UP		2
#define STATE_DOWN		3
#define STATE_EXPLODE	4

class cPlayer;

class cMonster : public cBicho
{
public:
	cMonster(void);
	virtual ~cMonster(void);

	virtual void Die();
	bool Appears();
	virtual void Draw(int tex_id);
	virtual void Logic(int *map, cPlayer &player, std::vector<cBlock*> &blocks);
};

