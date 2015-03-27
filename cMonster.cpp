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