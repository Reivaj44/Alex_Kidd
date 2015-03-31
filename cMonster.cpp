#include "cMonster.h"


cMonster::cMonster(void)
{
}


cMonster::~cMonster(void)
{
}

void cMonster::Die() 
{
	state = STATE_MONSTERDEAD;
}

bool cMonster::isDead() 
{
	return state == STATE_MONSTERDEAD;
}

void cMonster::Logic(int *map, cPlayer &player, std::vector<cBlock*> &blocks)
{
	if(state!=STATE_MONSTERDEAD && CollidesBox(player.GetBodyBox())) player.Die();
}
