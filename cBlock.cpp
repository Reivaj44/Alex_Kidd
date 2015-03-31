#include "cBlock.h"


cBlock::cBlock(void)
{
	state = STATE_BROWN;
	ibodybox.bottom=0; ibodybox.top=31;
	ibodybox.left=0; ibodybox.right=31;
}

cBlock::~cBlock(void)
{
}

bool cBlock::isDestroyed()
{
	return state==STATE_DESTROYED;
}

void cBlock::Destroy()
{
	state=STATE_DESTROYED;
}

void cBlock::Draw(int tex_id)
{	
	float xo,yo,xf,yf;
	switch(GetState())
	{
		case STATE_BROWN:	xo = 0.0f; yo = 0.5f;
								break;
		//1..4
		case STATE_GREEN:	xo = 0.250f; yo = 0.5f;
								break;
	}
	xf = xo + 0.250;
	yf = yo - 0.250f;
	DrawRect(tex_id,xo,yo,xf,yf);
}