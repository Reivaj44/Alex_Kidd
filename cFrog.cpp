#include "cFrog.h"


cFrog::cFrog(void)
{
	state = STATE_DOWN;
	ibodybox.left = 4; ibodybox.right = 31-4;
	ibodybox.bottom = 8; ibodybox.top = 31-8;
}


cFrog::~cFrog(void)
{
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
				xo = 0.0f + (GetFrame()*0.125f); yo = 0.875f;
				NextFrame(2);
				bottom = true;
				break;
			case STATE_UP:
				xo = 0.125f + (GetFrame()*0.125f); yo = 0.875f;
				NextFrame(2);
				break;
			case STATE_LEFT:	xo = 0.125f + (GetFrame()*0.125f); yo = 0.125f;
									NextFrame(2);
									break;
			//1..4
			case STATE_RIGHT:	xo = 0.0f + (GetFrame()*0.125f); yo = 0.125f;
									NextFrame(2);
									break;
		}
		xf = xo + 0.125;
		yf = yo - 0.125f;
		DrawRect(tex_id,xo,yo,xf,yf);
	}
}

void cFrog::Logic(int *map, cPlayer &player, std::vector<cBlock*> &blocks, const cRect &rectangle) {
	if(state!=STATE_EXPLODE)
	{
		int xaux;
		int yaux;
		bool bottom = false;
		if(state==STATE_DOWN) bottom = true;
		yaux = y;
		xaux = x;
		if(bottom) y += step_length;
		else y -= step_length;
		UpdateBox();

		if(CollidesMapWall(map,bottom,blocks, rectangle) || CollidesMapWall(map,!bottom,blocks, rectangle)) {
			x = xaux;
			UpdateBox();
			if(bottom) state=STATE_LEFT;
			else state=STATE_RIGHT;
			seq = 0;
			delay = 0;
		}
		cMonster::Logic(map,player,blocks, rectangle);
	}
}