#include "cScorpion.h"


cScorpion::cScorpion(void)
{
	state = STATE_LEFT;
	ibodybox.left = 9; ibodybox.right = 31-9;
	ibodybox.bottom = 13; ibodybox.top = 31-13;
}


cScorpion::~cScorpion(void)
{
}

void cScorpion::Draw(int tex_id)
{
	if(state==STATE_EXPLODE) cMonster::Draw(tex_id);
	else
	{
		float xo,yo,xf,yf;
		switch(GetState())
		{
			case STATE_LEFT:	xo = 0.0f + (GetFrame()*0.125f); yo = 0.75f;
									NextFrame(2);
									break;
		}
		xf = xo + 0.125;
		yf = yo - 0.125f;
		DrawRect(tex_id,xo,yo,xf,yf);
	}
}

void cScorpion::Logic(int *map, cPlayer &player, std::vector<cBlock*> &blocks, const cRect &rectangle, int level_width) {
	if(state!=STATE_EXPLODE)
	{
		x -= step_length;
		UpdateBox();
		cMonster::Logic(map,player,blocks, rectangle, level_width);
	}
}
