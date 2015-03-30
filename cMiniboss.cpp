
#include "cMiniboss.h"
#include "cScene.h"


cMiniboss::cMiniboss(void)
{
	state = STATE_UP;
	ibodybox.left = 6; ibodybox.right = 31-5;
	ibodybox.bottom = 0; ibodybox.top = 31-0;
	yact = 0;
}


cMiniboss::~cMiniboss(void)
{
}

void cMiniboss::Draw(int tex_id)
{	
	float xo,yo,xf,yf;
	if(state==STATE_UP || state==STATE_DOWN) 
	{
		xo = 0.0f + (GetFrame()*0.125f);
		yo = 0.5f;
		NextFrame(2);
	}
	xf = xo + 0.125;
	yf = yo - 0.125f;
	DrawRect(tex_id,xo,yo,xf,yf);
}

void cMiniboss::Logic(int *map, cPlayer &player) {
	if(state == STATE_UP) 
	{
		if( (yact) < 3*TILE_SIZE) 
		{
			y+=STEP_LENGTH;
			yact+=STEP_LENGTH;
		}
		else state = STATE_DOWN;
	}
	else
	{
		if( (yact) > 0) 
		{
			y-=STEP_LENGTH;
			yact-=STEP_LENGTH;
		}
		else state = STATE_UP;
	}

	cMonster::Logic(map,player);
}
