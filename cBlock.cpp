#include "cBlock.h"


cBlock::cBlock(void)
{
	state = STATE_BROWN;
	ibodybox.bottom=0; ibodybox.top=15;
	ibodybox.left=0; ibodybox.right=15;
	SetWidthHeight(16,16);
}

cBlock::~cBlock(void)
{
}

bool cBlock::Appears()
{
	return state!=STATE_DESTROYED;
}

bool cBlock::isCollisionable()
{
	return state!=STATE_DESTROYED;
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

void cBlock::Logic(cPlayer player, int &money, bool &ring, int &lifes, std::vector<cMonster*> &monsters)
{
}