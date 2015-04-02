#pragma once

#include "cMonster.h"

class cGhost : public cMonster
{
public:
	cGhost(void);
	virtual ~cGhost(void);

	virtual void Die();

	virtual void Draw(int tex_id);
	virtual void Logic(int *map, cPlayer &player, std::vector<cBlock*> &blocks);

private:
	float xdir, ydir;
	int wait;
	int retard;
};

