#include "cBox.h"


cBox::cBox(void)
{
	state = STATE_STAR;
	collisionable = true;
	appears = true;
	ibodybox.bottom=0; ibodybox.top=15;
	ibodybox.left=0; ibodybox.right=15;
}


cBox::~cBox(void)
{
}

void cBox::SetTreasure(int treas)
{
	treasure=treas;
}

int cBox::GetTreasure()
{
	return treasure;
}

bool cBox::Appears()
{
	return appears;
}

bool cBox::isCollisionable()
{
	return collisionable;
}

void cBox::Destroy()
{
	if(collisionable && state!=STATE_SKULL)
		SetState(treasure);
}

void cBox::SetState(int st) 
{
	cBicho::SetState(st);
	if(st<4) collisionable = false;
}

void cBox::Draw(int tex_id)
{	
	float xo,yo,xf,yf;
	switch(GetState())
	{
		case STATE_SKULL:	xo = 0.0f; yo = 0.25f;
								break;
		//1..4
		case STATE_QUEST:	xo = 0.250f; yo = 0.25f;
								break;

		case STATE_STAR:	xo = 0.5f; yo = 0.25f;
								break;

		case STATE_BMON:	xo = 0.5f; yo = 0.5f;
								break;

		case STATE_SMON:	xo = 0.75f; yo = 0.5f;
								break;

		case STATE_RING:	xo = 0.0f; yo = 0.5f;
								break;
	}
	xf = xo + 0.250;
	yf = yo - 0.250f;
	DrawRect(tex_id,xo,yo,xf,yf);
}

void cBox::Logic(cPlayer player)
{
	if(!collisionable && CollidesBox(player.GetBodyBox())) appears=false;

}
