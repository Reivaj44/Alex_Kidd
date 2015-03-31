#pragma once

#include "cMonster.h"

class cMiniboss : public cMonster
{
public:
	cMiniboss(void);
	virtual ~cMiniboss(void);

	virtual void Draw(int tex_id);
	virtual void Logic(int *map, cPlayer &player, std::vector<cBlock*> &blocks);

private:
	int yact;
};

