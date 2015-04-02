#include "cBox.h"


cBox::cBox(void)
{
	state = STATE_STAR;
	collisionable = true;
	appears = true;
	ring_appeared = false;
	poisoned = false;
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
	return state!=STATE_DISAPPEARED;
}

bool cBox::isCollisionable()
{
	return collisionable;
}

void cBox::Destroy()
{
	if(collisionable)
	{
		if(state!=STATE_SKULL && state!=STATE_SKULL_P)
		{
			if(treasure==STATE_RING) ring_appeared = true;
			SetState(treasure);
		}
		else poisoned = true;
	}
}

void cBox::SetState(int st) 
{
	cBicho::SetState(st);
	if(st<=4) collisionable = false;
}

void cBox::Draw(int tex_id)
{	
	float xo,yo,xf,yf;
	switch(GetState())
	{
		case STATE_SKULL:	xo = 0.0f; yo = 0.25f;
								break;

		case STATE_QUEST:	xo = 0.250f; yo = 0.25f;
								break;

		case STATE_STAR:	xo = 0.5f; yo = 0.25f;
								break;

		case STATE_SKULL_P:	xo = 0.75f; yo = 0.25f;
								break;

		case STATE_BMON:	xo = 0.5f; yo = 0.5f;
								break;

		case STATE_SMON:	xo = 0.75f; yo = 0.5f;
								break;

		case STATE_RING:	xo = 0.0f; yo = 0.75f;
								break;

		case STATE_LIFE:	xo = 0.5f; yo= 0.75f;
								break;

	}
	xf = xo + 0.250;
	yf = yo - 0.250f;
	DrawRect(tex_id,xo,yo,xf,yf);
}

void cBox::Logic(cPlayer player, int &money, int &ring, int &lifes)
{
	if(state==STATE_SKULL_P)
	{
		cRect BodyBoxExt=player.GetBodyBox();
		BodyBoxExt.bottom--; BodyBoxExt.top++;
		BodyBoxExt.left--; BodyBoxExt.right++;
		if(CollidesBox(BodyBoxExt))
		{
			//afegir fantasma i mirar d'eliminar el segon fantasma
		}
	}
	if(ring_appeared)
	{
		if(ring==HV_RING || ring==AP_RING)
		{
			SetState(STATE_DISAPPEARED);
			//afegir fantasma i mirar d'eliminar el segon fantasma
		}
		else ring = AP_RING;
		ring_appeared = false;
	}
	else if(poisoned)
	{
		player.Poison();
		poisoned = false;
	}
	if(state!=STATE_DISAPPEARED && !collisionable && CollidesBox(player.GetBodyBox())) 
	{
		switch(GetState())
		{
			case STATE_BMON: money+=20; break;
			case STATE_SMON: money+=10; break;
			case STATE_RING: ring = HV_RING; break;
			case STATE_LIFE: lifes++; break;
		}
		SetState(STATE_DISAPPEARED);
	}

}
