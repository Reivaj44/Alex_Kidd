#include "cGhost.h"


cGhost::cGhost(void)
{
	xdir = 0;
	ydir = 0;
	state = STATE_LEFT;
	wait = 0;
	retard = 0;
	ibodybox.left = 8; ibodybox.right = 31-8;
	ibodybox.bottom = 8; ibodybox.top = 31-8;
}


cGhost::~cGhost(void)
{
}

void cGhost::Die()
{
}

bool cGhost::Appears(const cRect &cam)
{
	bool appear = cBicho::Appears(cam);
	if(!appear) state = STATE_DISAPPEARED;
	return appear;
}

void cGhost::Draw(int tex_id)
{	
	float xo,yo,xf,yf;
	bool left = false;
	switch(GetState())
	{
		case STATE_LEFT:	xo = 0.0f + (GetFrame()*0.125f); yo = 0.625f;
								NextFrame(2);
								left = true; break;
		//1..4
		case STATE_RIGHT:	xo = 0.125f + (GetFrame()*0.125f); yo = 0.625f;
								NextFrame(2);
								break;
	}
	xf = xo - 0.125;
	if(left) xf = xo + 0.125;
	yf = yo - 0.125f;
	DrawRect(tex_id,xo,yo,xf,yf);
}

void cGhost::Logic(int *map, cPlayer &player, std::vector<cBlock*> &blocks) {
	if(wait>100) 
	{
		retard %= 30;
		if(retard==0 && !player.isDead()) 
		{
			int xplayer, yplayer;
			player.GetPosition(xplayer, yplayer);
			xdir = xplayer - bodybox.left;
			ydir = yplayer - bodybox.bottom;
			float modulo = sqrt(float(xdir*xdir + ydir*ydir));
			xdir /= (modulo);
			ydir /= (modulo);
			if(xdir>0) state = STATE_RIGHT;
			else state = STATE_LEFT;
		}
		x+=xdir;
		y+=ydir;
		retard++;
		UpdateBox();
		cMonster::Logic(map,player, blocks);
	}
	else wait++;
}