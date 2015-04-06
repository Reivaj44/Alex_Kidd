#include "cFrog.h"


cFrog::cFrog(void)
{
	SetState(STATE_DOWN);
	y_ini = 0;
	down_delay = 0;
	get_top = false;
}


cFrog::~cFrog(void)
{
}

void cFrog::SetState(int st)
{
	if(st==STATE_DOWN)
	{
		ibodybox.left = 8; ibodybox.right = 31-8;
		ibodybox.bottom = 4; ibodybox.top = 31-12;
		UpdateBox();	
	}
	else if(st==STATE_DOWN)
	{
		ibodybox.left = 8; ibodybox.right = 31-8;
		ibodybox.bottom = 4; ibodybox.top = 31-4;
		UpdateBox();
	}
	state=st;
}
			
void cFrog::Draw(int tex_id)
{
	if(state==STATE_EXPLODE) cMonster::Draw(tex_id);
	else
	{
		float xo,yo,xf,yf;
		bool bottom = false;
		switch(GetState())
		{
			case STATE_DOWN:
				xo = 0.0f; yo = 0.875f;
				break;
			case STATE_UP:
				xo = 0.125f; yo = 0.875f;
				break;
		}
		xf = xo + 0.125;
		yf = yo - 0.125f;
		DrawRect(tex_id,xo,yo,xf,yf);
	}
}

void cFrog::Logic(int *map, cPlayer &player, std::vector<cBlock*> &blocks, const cRect &rectangle, int level_width, int& lifes) {
	if(state!=STATE_EXPLODE)
	{
		
		if(state==STATE_DOWN) 
		{
			++down_delay;
			if(down_delay==30)
			{
				y_ini = 0;
				SetState(STATE_UP);
			}
		}

		if(state==STATE_UP)
		{
			if(y_ini<32)
			{
				y += step_length*2;
				UpdateBox();
				y_ini += step_length*2;
			}
			else
			{
				y-=step_length*1.5;
				UpdateBox();
				if(CollidesMapFloor(map,blocks,level_width)) 
				{
					down_delay=0;
					SetState(STATE_DOWN);
				}
			}
		}
		cMonster::Logic(map,player,blocks, rectangle, level_width, lifes);
	}
}