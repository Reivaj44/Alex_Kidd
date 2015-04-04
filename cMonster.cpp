#include "cMonster.h"


cMonster::cMonster(void)
{
	SetWidthHeight(32,32);
	frame_delay*=2;
	step_length/=4;
}


cMonster::~cMonster(void)
{
}

bool cMonster::isDead()
{
	return (state==STATE_DISAPPEARED || state==STATE_EXPLODE);
}

void cMonster::Die() 
{
	mciSendString("play SOUNDS/dead_enemie.wav", NULL, 0, NULL);
	state = STATE_EXPLODE;
	seq = 0;
	delay = 0;
}

void cMonster::Draw(int tex_id)
{
	float xo,yo,xf,yf;
	xo = 0.0f + (GetFrame()*0.125f); yo = 1.0f;
	
	delay++;
	if(delay == (frame_delay))
	{
		seq++;
		delay = 0;
	}
	if(seq==2) state = STATE_DISAPPEARED;	

	xf = xo + 0.125;
	yf = yo - 0.125f;
	DrawRect(tex_id,xo,yo,xf,yf);
}

void cMonster::Logic(int *map, cPlayer &player, std::vector<cBlock*> &blocks, const cRect &rectangle)
{
	if(state!=STATE_EXPLODE && CollidesBox(player.GetBodyBox())) player.Die();
}
