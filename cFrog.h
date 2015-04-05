#pragma once
#include "cMonster.h"

class cFrog : public cMonster
{
public:
	cFrog(void);
	virtual ~cFrog(void);

	virtual void Draw(int tex_id);
	virtual void Logic(int *map, cPlayer &player, std::vector<cBlock*> &blocks, const cRect &rectangle);
};

