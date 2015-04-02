#pragma once
#include "cBox.h"
#include "cGhost.h"


int cBox::rings_appeared = 0;

cBox::cBox(void)
{
	state = STATE_STAR;
	collisionable = true;
	appears = true;
	poisoned = false;
	ghost_appears = false;
	touching = false;
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
			SetState(treasure);
			if(treasure==STATE_RING)
			{
				if(rings_appeared==1) 
				{
					SetState(STATE_DISAPPEARED);
					ghost_appears = true;
				}
				else rings_appeared++;
			}
		}
		else 
		{
			SetState(STATE_DISAPPEARED);
			poisoned = true;
		}
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

void cBox::Logic(cPlayer &player, int &money, bool &ring, int &lifes, std::vector<cMonster*> &monsters)
{
	if(state==STATE_SKULL_P)
	{
		cRect BodyBoxExt=player.GetBodyBox();
		BodyBoxExt.bottom--; BodyBoxExt.top++;
		BodyBoxExt.left--; BodyBoxExt.right++;
		if(CollidesBox(BodyBoxExt)) {
			if(!touching) ghost_appears = true;
			touching = true;
		}
		else touching = false;
	}
	if(ghost_appears)
	{
		cMonster* Ghost = new cGhost();
		int aux_x, aux_y;
		GetTile(aux_x,aux_y);
		Ghost->SetTile(aux_x,aux_y);
		monsters.push_back(Ghost);
		ghost_appears = false;
	}
	if(poisoned)
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
			case STATE_RING: ring = true; break;
			case STATE_LIFE: lifes++; break;
		}
		SetState(STATE_DISAPPEARED);
	}

}

