#pragma once

#include "cMonster.h"

class cMiniboss : public cMonster
{
public:
	cMiniboss(void);
	virtual ~cMiniboss(void);

	virtual void Die();
	virtual void Draw(int tex_id);
	virtual void Logic(int *map, cPlayer &player, std::vector<cBlock*> &blocks);

private:
	float yact;
	int lifes;
	bool touched;
};

