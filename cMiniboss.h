#pragma once

#include "cMonster.h"

class cMiniboss : public cMonster
{
public:
	cMiniboss(void);
	virtual ~cMiniboss(void);

	virtual void Die();
	virtual void Draw(int tex_id);
	virtual void Logic(int *map, cPlayer &player, std::vector<cBlock*> &blocks, const cRect &rectangle, int level_width);

private:
	float yact;
	int lifes;
	bool touched;
	bool booblechange;

	class cBooble
	{
	public:
		cRect bodybox;
		float dir_x;
		float dir_y;
		float inc_y;
	};

	std::vector<cBooble*> boobles;

	bool CollidesBoobles(cRect &playerbox);
	void DrawBoobles(int tex_id);
};

