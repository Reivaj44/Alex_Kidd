#include "cGhost.h"


cGhost::cGhost(void)
{
	state = STATE_RIGHT;
	ibodybox.left = 8; ibodybox.right = 31-8;
	ibodybox.bottom = 8; ibodybox.top = 31-8;
}


cGhost::~cGhost(void)
{
}

void cGhost::Die()
{
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

void cGhost::Logic(int *map, cPlayer &player) {
	delay %= 17;
	if(delay==0) 
	{
		int xplayer, yplayer;
		player.GetPosition(xplayer, yplayer);
		xdir = xplayer - bodybox.left;
		ydir = yplayer - bodybox.bottom;
		float modulo = sqrt(float(xdir*xdir + ydir*ydir));
		xdir /= (modulo/2.0f);
		ydir /= (modulo/2.0f);
		if(xdir>0) state = STATE_RIGHT;
		else state = STATE_LEFT;
	}
	x+=xdir;
	y+=ydir;
	delay++;
	UpdateBox();

	cMonster::Logic(map,player);
}