#include "cPtero.h"
#include "Globals.h"
#include "cScene.h"


cPtero::cPtero(void)
{
	state = STATE_RIGHT;
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
		case STATE_LEFT:	xo = 0.375f + (GetFrame()*0.5f); yo = 1.0f;
								NextFrame(2);
								left = true; break;
		//1..4
		case STATE_RIGHT:	xo = 0.0f + (GetFrame()*0.5f); yo = 1.0f;
								NextFrame(2);
								break;
	}
	xf = xo + 0.375;
	if(left) xf = xo - 0.375;
	yf = yo - 1.0f;
	DrawRect(tex_id,xo,yo,xf,yf);
}

void cPtero::Logic(int *map) {
	int xaux;
	bool right = false;
	if(state==STATE_RIGHT) right = true;
	xaux = x;
	if(right) x += STEP_LENGTH;
	else x -= STEP_LENGTH;
	UpdateBox();

	if(CollidesMapWall(map,right) || CollidesMapWall(map,!right)) {
		x = xaux;
		UpdateBox();
		if(right) state=STATE_LEFT;
		else state=STATE_RIGHT;
		seq = 0;
		delay = 0;
	}	
}