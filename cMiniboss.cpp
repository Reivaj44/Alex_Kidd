
#include "cMiniboss.h"
#include "cScene.h"


cMiniboss::cMiniboss(void)
{
	state = STATE_UP;
	ibodybox.left = 6; ibodybox.right = 31-5;
	ibodybox.bottom = 0; ibodybox.top = 31-0;
	yact = 0;
	lifes = 2;
	touched = false;
}


cMiniboss::~cMiniboss(void)
{
}

void cMiniboss::Die()
{
	if(!touched)
	{
		lifes--;
		if(lifes==0) state = STATE_EXPLODE;
		touched = true;
	}
}

void cMiniboss::Draw(int tex_id)
{	
	if(state==STATE_EXPLODE) cMonster::Draw(tex_id);
	else
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
}

void cMiniboss::Logic(int *map, cPlayer &player, std::vector<cBlock*> &blocks) {
	if(state!=STATE_EXPLODE) {
		if(state == STATE_UP) 
		{
			if( (yact) < 3*TILE_SIZE) 
			{
				y+=step_length;
				yact+=step_length;
				UpdateBox();
			}
			else state = STATE_DOWN;
		}
		else
		{
			if( (yact) > 0) 
			{
				y-=step_length;
				yact-=step_length;
				UpdateBox();
			}
			else state = STATE_UP;
		}
		if(touched && !CollidesBox(player.GetHitBox())) touched = false;
	}

	cMonster::Logic(map,player, blocks);
}
