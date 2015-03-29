#pragma once
#include "cBicho.h"
#include "cPlayer.h"

#define STATE_LEFT		0
#define STATE_RIGHT		1
#define STATE_UP		2
#define STATE_DOWN		3
#define STATE_MONSTERDEAD		4

class cPlayer;

class cMonster :
	public cBicho
{
public:
	cMonster(void);
	virtual ~cMonster(void);

	void Die();
	bool isDead();
	virtual void Draw(int tex_id) = 0;
	virtual void Logic(int *map, cPlayer &player) = 0;
};

