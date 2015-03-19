#include "cPtero.h"
#include "Globals.h"
#include "cScene.h"


cPtero::cPtero(void)
{
	SetState(STATE_WALKRIGHT);
}


cPtero::~cPtero(void)
{
}

void cPtero::Draw(int tex_id)
{	
	float xo,yo,xf,yf;
	bool left = false;
	switch(GetState())
	{
		case STATE_WALKLEFT:	xo = 0.375f + (GetFrame()*0.5f); yo = 1.0f;
								NextFrame(2);
								left = true; break;
		//1..4
		case STATE_WALKRIGHT:	xo = 0.0f + (GetFrame()*0.5f); yo = 1.0f;
								NextFrame(2);
								break;
	}
	xf = xo + 0.375;
	if(left) xf = xo - 0.375;
	yf = yo - 1.0f;
	DrawRect(tex_id,xo,yo,xf,yf);
}

void cPtero::Move(int *map) {
	int xaux;
	int right = false;
	if(state==STATE_WALKRIGHT) right = true;

	//Whats next tile?
	/*if( (x % TILE_SIZE) == 0)
	{*/
		xaux = x;
		if(right) x += STEP_LENGTH;
		else x -= STEP_LENGTH;

		if(CollidesMapWall(map,right) || CollidesMapWall(map,!right)) {
			x = xaux;
			if(right) state=STATE_WALKLEFT;
			else state=STATE_WALKRIGHT;
			seq = 0;
			delay = 0;
		}
	/*}
	//Advance, no problem
	else
	{
		if(right) x += STEP_LENGTH;
		else x -= STEP_LENGTH;
	}*/	
}