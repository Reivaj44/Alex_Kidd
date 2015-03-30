#include "cSFish.h"


cSFish::cSFish(void)
{
	ibodybox.left = 8; ibodybox.right = 31-8;
	ibodybox.bottom = 9; ibodybox.top = 31-9;
}


cSFish::~cSFish(void)
{
}

void cSFish::Draw(int tex_id)
{	
	float xo,yo,xf,yf;
	bool left = false;
	switch(GetState())
	{
		case STATE_LEFT:	xo = 0.125f + (GetFrame()*0.125f); yo = 0.250f;
								NextFrame(2);
								left = true; break;
		//1..4
		case STATE_RIGHT:	xo = 0.0f + (GetFrame()*0.125f); yo = 0.250f;
								NextFrame(2);
								break;
	}
	xf = xo + 0.125;
	if(left) xf = xo - 0.125;
	yf = yo - 0.125f;
	DrawRect(tex_id,xo,yo,xf,yf);
}
