#pragma once
#include "cmonster.h"
class cScorpion : public cMonster
{

public:
	cScorpion(void);
	virtual ~cScorpion(void);

	virtual void Draw(int tex_id);
	virtual void Logic(int *map, cPlayer &player, std::vector<cBlock*> &blocks, const cRect &rectangle, int level_width);
};

