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
	destroying = false;
	green = false;
	ydes = 0;
	xinc = 2.5;
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

bool cBlock::Appears(const cRect &cam)
{
	if ((ydes+h)<cam.bottom) destroying = false;
	return (destroying || cBicho::Appears(cam));
}

bool cBlock::isCollisionable()
{
	return collisionable;
}

void cBlock::Destroy()
{
	if(collisionable)
	{
		if(state==R_BROWN || state==R_GREEN) mciSendString("play SOUNDS/smb_breakblock.wav", NULL, 0, NULL);
		else mciSendString("play SOUNDS/smb3_fireball.wav", NULL, 0, NULL);
		if(state!=SKULL && state!=SKULL_P && state!=R_BROWN && state!=R_GREEN)
		{
			SetState(treasure);
			destroying = true;
			xdes = x; ydes = y;
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
			destroying = true;
			xdes = x; ydes = y;
			if(state==SKULL) poisoned = true;
			SetState(STATE_DISAPPEARED);
		}
	}
}

void cBlock::SetState(int st) 
{
	cBicho::SetState(st);
	if(st<=4 || st>=11) {
		collisionable = false;
		if(st<=4) y--;
	}
	if(st==R_GREEN) green = true;
}

void cBlock::Draw(int tex_id)
{	
	if(destroying) DrawRock(tex_id);
	if(state!=STATE_DISAPPEARED)
	{
		float xo,yo,xf,yf;
		switch(GetState())
		{
			case SKULL:		xo = 0.0f; yo = 0.25f;
									break;

			case QUEST:		xo = 0.25f; yo = 0.25f;
									break;

			case STAR:		xo = 0.5f; yo = 0.25f;
									break;

			case SKULL_P:	xo = 0.75f; yo = 0.25f;
									break;

			case BMON:		xo = 0.5f; yo = 0.5f;
									break;

			case SMON:		xo = 0.75f; yo = 0.5f;
									break;

			case RING:		xo = 0.0f; yo = 0.75f;
									break;

			case LIFE:		xo = 0.5f; yo= 0.75f;
									break;

			case R_BROWN:	xo = 0.0f; yo = 0.5f;
									break;

			case R_GREEN:	xo = 0.25f; yo = 0.5f;
									break;

			case CHBX:		xo = 0.25f; yo = 1.0f;
									break;

			case CHBXT:		xo = 0.5f; yo = 1.0f;
									break;

			case RICE:		xo = 0.25f; yo = 0.75f;
									break;
		}
		xf = xo + 0.250;
		yf = yo - 0.250f;
		DrawRect(tex_id,xo,yo,xf,yf);
	}
}

void cBlock::Logic(cPlayer &player, int &money, int &lifes, std::vector<cMonster*> &monsters, int &check_x, int &check_y, bool &level_completed)
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
			case BMON: money+=20; Money(money, lifes); break;
			case SMON: money+=10; Money(money, lifes); break;
			case RING: player.PowerUp(); mciSendString("play SOUNDS/smb_powerup.wav", NULL, 0, NULL); break;
			case LIFE: lifes++; mciSendString("play SOUNDS/smb_1-up.wav", NULL, 0, NULL); break;
			case CHBX: GetTile(check_x,check_y); mciSendString("play SOUNDS/smb3_pause.wav", NULL, 0, NULL); SetState(CHBXT); break;
			case RICE: level_completed = true; mciSendString("play SOUNDS/rice_ball.wav", NULL, 0, NULL); break;
		}
		if(state!=CHBXT && state!=CHBX) SetState(STATE_DISAPPEARED);
	}

}

void cBlock::DrawRock(int tex_id)
{
	float xdes2 = x+(x-xdes);
	float xo = 0.0f; float yo = 1.0f;
	float xf = 0.125f; float yf = 0.75f;
	if(green)
	{
		xo+=0.125; xf+=0.125;
	}

	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D,tex_id);
	glBegin(GL_QUADS);	
		glTexCoord2f(xo,yo);	glVertex2i(xdes  , ydes);
		glTexCoord2f(xf,yo);	glVertex2i(xdes+w/2, ydes);
		glTexCoord2f(xf,yf);	glVertex2i(xdes+w/2, ydes+h);
		glTexCoord2f(xo,yf);	glVertex2i(xdes  , ydes+h);
	glEnd();

	glBegin(GL_QUADS);	
		glTexCoord2f(xo,yo);	glVertex2i(xdes2+w/2  , ydes);
		glTexCoord2f(xf,yo);	glVertex2i(xdes2+w, ydes);
		glTexCoord2f(xf,yf);	glVertex2i(xdes2+w, ydes+h);
		glTexCoord2f(xo,yf);	glVertex2i(xdes2+w/2  , ydes+h);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	delay++;
	if(delay==1)
	{
		ydes-=3;
		xdes-=xinc;
		if(xinc>0.5) xinc*=0.80;
		delay=0;
	}
}

void cBlock::Money(int &money, int &lifes)
{
	const int money_life = 30;
	if(money>=money_life)
	{
		lifes++;
		mciSendString("play SOUNDS/smb_1-up.wav", NULL, 0, NULL);
		money=money%money_life;
	}
	else mciSendString("play SOUNDS/smb_coin.wav", NULL, 0, NULL); 
}
