#pragma once

#include "cMonster.h"

class cPtero :	public cMonster
{
public:
	cPtero(void);
	virtual ~cPtero(void);

	virtual void Draw(int tex_id);
	virtual void Logic(int *map, cPlayer &player, std::vector<cBlock*> &blocks, const cRect &rectangle);

};

