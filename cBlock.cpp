#pragma once
#include "cGhost.h"
#include "cBlock.h"


int cBlock::rings_appeared = 0;

cBlock::cBlock(void)
{
	SetState(R_BROWN);
	ibodybox.bottom=0; ibodybox.top=15;
	ibodybox.left=0; ibodybox.right=15;
	SetWidthHeight(16,16);
	collisionable = true;
	appears = true;
	poisoned = false;
	ghost_appears = false;
	touching = false;
}

cBlock::~cBlock(void)
{
}

void cBlock::SetTreasure(int treas)
{
	treasure=treas;
}

int cBlock::GetTreasure()
{
	return treasure;
}

bool cBlock::Appears()
{
	return state!=STATE_DISAPPEARED;
}

bool cBlock::isCollisionable()
{
	return collisionable;
}

void cBlock::Destroy()
{
	if(collisionable)
	{
		if(state!=SKULL && state!=SKULL_P && state!=R_BROWN && state!=R_GREEN)
		{
			SetState(treasure);
			if(treasure==RING)
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
			if(state==SKULL) poisoned = true;
			SetState(STATE_DISAPPEARED);
		}
	}
}

void cBlock::SetState(int st) 
{
	cBicho::SetState(st);
	if(st<=4) {
		collisionable = false;
		y--;
	}
}

void cBlock::Draw(int tex_id)
{	
	float xo,yo,xf,yf;
	switch(GetState())
	{
		case SKULL:	xo = 0.0f; yo = 0.25f;
								break;

		case QUEST:	xo = 0.250f; yo = 0.25f;
								break;

		case STAR:	xo = 0.5f; yo = 0.25f;
								break;

		case SKULL_P:	xo = 0.75f; yo = 0.25f;
								break;

		case BMON:	xo = 0.5f; yo = 0.5f;
								break;

		case SMON:	xo = 0.75f; yo = 0.5f;
								break;

		case RING:	xo = 0.0f; yo = 0.75f;
								break;

		case LIFE:	xo = 0.5f; yo= 0.75f;
								break;

		case R_BROWN:	xo = 0.0f; yo = 0.5f;
								break;

		case R_GREEN:	xo = 0.250f; yo = 0.5f;
								break;
	}
	xf = xo + 0.250;
	yf = yo - 0.250f;
	DrawRect(tex_id,xo,yo,xf,yf);
}

void cBlock::Logic(cPlayer &player, int &money, bool &ring, int &lifes, std::vector<cMonster*> &monsters)
{
	if(state==SKULL_P)
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
			case BMON: money+=20; break;
			case SMON: money+=10; break;
			case RING: ring = true; break;
			case LIFE: lifes++; break;
		}
		SetState(STATE_DISAPPEARED);
	}

}