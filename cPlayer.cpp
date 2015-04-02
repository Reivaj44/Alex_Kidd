
#include "cPlayer.h"
#include "Globals.h"

cPlayer::cPlayer() 
{
	intheair = false;
	jumping = false;
	left = false;
	ibodybox.left = 10; ibodybox.right = 31 - 10;
	ibodybox.bottom = 5; ibodybox.top = 31 - 5;
}
cPlayer::~cPlayer(){}

void cPlayer::SetWidthHeight(int width,int height)
{
	w = width;
	h = height;
	UpdateBox();
}

cRect cPlayer::GetHitBox()
{
	return punchbox;
}

bool cPlayer::isPunching()
{
	return punching;
}

bool cPlayer::isDead()
{
	return state==STATE_DEAD;
}

void cPlayer::UpdateBox() 
{
	cBicho::UpdateBox();
	
	punchbox.bottom = y + ipunchbox.bottom;
	punchbox.top = y + ipunchbox.top;
	punchbox.left = x + ipunchbox.left;
	punchbox.right = x + ipunchbox.right;

	if(left)
	{
		punchbox.left = x + 31 - ipunchbox.right;
		punchbox.right = x + 31 - ipunchbox.left;
	}
}

void cPlayer::ChangeBox()
{
	if(punching) 
	{
		ipunchbox.left = 22; ipunchbox.right = 30;
		ipunchbox.bottom = 12; ipunchbox.top = 19;
	}
	else
	{
		ipunchbox.left = 0; ipunchbox.right = 0;
		ipunchbox.bottom = 0; ipunchbox.top = 0;
	}
	UpdateBox();
}

void cPlayer::MoveLeft(int *map, std::vector<cBlock*> &blocks)
{
	if(state != STATE_DEAD)
	{
		if(state==STATE_CROUCHLEFT || state==STATE_CROUCHRIGHT)	SetState(STATE_CROUCHLEFT);
		else if (!punching || intheair)
		{
			int xaux;
			//Whats next tile?
		
			xaux = x;
			x -= STEP_LENGTH;
			UpdateBox();

			if(CollidesMapWall(map,false,blocks)) 
			{
				x = xaux;
				UpdateBox();
			}
			else if(state != STATE_WALKLEFT)
			{
				seq = 0;
				delay = 0;
			}
			if(intheair && !punching) SetState(STATE_JUMPLEFT);
			else if(!punching) SetState(STATE_WALKLEFT);
			else if(intheair) SetState(STATE_PUNCHLEFT);
		}
	}
}

void cPlayer::MoveRight(int *map, std::vector<cBlock*> &blocks)
{
	if(state!=STATE_DEAD) {
		if(state==STATE_CROUCHRIGHT || state==STATE_CROUCHLEFT) SetState(STATE_CROUCHRIGHT);
		else if (!punching || intheair)
		{
			int xaux;
			//Whats next tile?
			xaux = x;
			x += STEP_LENGTH;
			UpdateBox();

			
			if(CollidesMapWall(map,true,blocks)) {
				x = xaux;
				UpdateBox();
			}

			else if(state != STATE_WALKRIGHT)
			{
				seq = 0;
				delay = 0;
			}

			if(intheair && !punching) SetState(STATE_JUMPRIGHT);
			else if(!punching) SetState(STATE_WALKRIGHT);	
			else if(intheair) SetState(STATE_PUNCHRIGHT);
		}
	}
}

void cPlayer::Crouch(int *map)
{
	if(!intheair && state!=STATE_DEAD) {
		/*if(CollidesMapWall(map,true) && state==STATE_WALKRIGHT) SetState(STATE_CROUCHRIGHT);
		else if(CollidesMapWall(map,false) && state==STATE_WALKLEFT) SetState(STATE_CROUCHLEFT);
		else
		{*/
			switch(state)
			{
				case STATE_LOOKLEFT:	SetState(STATE_CROUCHLEFT);		break;
				case STATE_LOOKRIGHT:	SetState(STATE_CROUCHRIGHT);		break;
			}
		//}
	}
}

void cPlayer::Punch(int *map)
{
	if(state!=STATE_CROUCHLEFT && state!=STATE_CROUCHRIGHT && !punching && state!=STATE_DEAD)
	{
		punching = true;
		if(!left) SetState(STATE_PUNCHRIGHT);
		else SetState(STATE_PUNCHLEFT);
		seq = 0;
		delay = 0;
	}
}

void cPlayer::Stop()
{
	if(!intheair && !punching && state!=STATE_DEAD){
		switch(state)
		{
			case STATE_WALKLEFT:	SetState(STATE_LOOKLEFT);		break;
			case STATE_WALKRIGHT:	SetState(STATE_LOOKRIGHT);	break;
			case STATE_JUMPLEFT:	SetState(STATE_LOOKLEFT);		break;
			case STATE_JUMPRIGHT:	SetState(STATE_LOOKRIGHT);	break;
			case STATE_CROUCHLEFT:	SetState(STATE_LOOKLEFT);		break;
			case STATE_CROUCHRIGHT:	SetState(STATE_LOOKRIGHT);	break;
			case STATE_PUNCHLEFT:	SetState(STATE_LOOKLEFT);		break;
			case STATE_PUNCHRIGHT:	SetState(STATE_LOOKRIGHT);	break;
		}
	}
	else if(intheair && !punching && state!=STATE_DEAD) {
		switch(state) 
		{
			case STATE_WALKLEFT:		SetState(STATE_JUMPLEFT);		break;
			case STATE_WALKRIGHT:		SetState(STATE_JUMPRIGHT);	break;
			case STATE_LOOKRIGHT:		SetState(STATE_JUMPRIGHT);	break;
			case STATE_LOOKLEFT:		SetState(STATE_JUMPLEFT);		break;
			case STATE_PUNCHLEFT:		SetState(STATE_JUMPLEFT);		break;
			case STATE_PUNCHRIGHT:		SetState(STATE_JUMPRIGHT);	break;
		}
	}
		
}

void cPlayer::Jump(int *map)
{
	if(!jumping)
	{
		if(!intheair && (state!=STATE_CROUCHLEFT) && (state!=STATE_CROUCHRIGHT) && !punching)
		{
			if(state==STATE_WALKLEFT || state==STATE_LOOKLEFT) SetState(STATE_JUMPLEFT);
			else if(state==STATE_WALKRIGHT || state==STATE_LOOKRIGHT) SetState(STATE_JUMPRIGHT);
			jumping = true;
			intheair = true;
			jump_alfa = 0;
			jump_y = y;
		}
	}
}

void cPlayer::Die()
{
	SetState(STATE_DEAD);
}

void cPlayer::Poison()
{
	//paralitzar durant x segons
}

void cPlayer::SetState(int s)
{
	cBicho::SetState(s);
	if(state==STATE_LOOKLEFT || state==STATE_WALKLEFT || state==STATE_JUMPLEFT || state==STATE_CROUCHLEFT || state==STATE_PUNCHLEFT) left = true;
	else left = false;
	ChangeBox();
}

void cPlayer::Logic(int *map, std::vector<cMonster*> &monsters, std::vector<cBlock*> &blocks)
{
	if(state==STATE_DEAD) {
		y += 2;
		UpdateBox();
	}

	else {
		if(punching) {
			for(unsigned int i = 0; i < monsters.size(); i++) 
				if(!monsters[i]->isDead() && monsters[i]->CollidesBox(punchbox)) monsters[i]->Die();
			for(unsigned int i = 0; i < blocks.size(); i++) 
				if(blocks[i]->isCollisionable() && blocks[i]->CollidesBox(punchbox)) blocks[i]->Destroy();

			delay++;
			if(delay == FRAME_DELAY)
			{
				delay = 0;
				punching = false;
			}
		}
		if(jumping)
		{
			float alfa;
			jump_alfa += JUMP_STEP;
		
			if(jump_alfa == 180)
			{
				jumping = false;
				y = jump_y;
				UpdateBox();
			}

			else 
			{
				alfa = ((float)jump_alfa) * 0.017453f;
				y = jump_y + (int)( ((float)JUMP_HEIGHT) * sin(alfa) );
				UpdateBox();
			
				if(jump_alfa <= 90 && (CollidesMapCeil(map,blocks)))
					jumping = false;

				if(jump_alfa > 90)
				{
					//Over floor?
					if(CollidesMapFloor(map,blocks))
					{	jumping=false;
						intheair=false;
					}
				}
			}
		}

		else
		{
			//Over floor?
			if(!CollidesMapFloor(map,blocks)) {
				y -= (2*STEP_LENGTH);
				UpdateBox();
				intheair=true;
			}

			else intheair=false;
		}
	}
}

void cPlayer::Draw(int tex_id)
{	
	float xo,yo,xf,yf;
	bool left = false;
	switch(GetState())
	{
		//5
		case STATE_LOOKLEFT:	xo = 0.625f;	yo = 0.125f;
								left = true; 
								break;
		//5
		case STATE_LOOKRIGHT:	xo = 0.500f;	yo = 0.125f;
								break;
		//4..1
		case STATE_WALKLEFT:	xo = 0.125f + (GetFrame()*0.125f); yo = 0.125;
								NextFrame(4);
								left = true; break;
		//1..4
		case STATE_WALKRIGHT:	xo = 0.0f + (GetFrame()*0.125f); yo = 0.125f;
								NextFrame(4);
								break;
		//6
		case STATE_JUMPLEFT:	xo = 0.750f; yo = 0.125f;
								left = true; break;
		//6
		case STATE_JUMPRIGHT:	xo = 0.625f; yo = 0.125;
								break;
		//7
		case STATE_CROUCHLEFT:	xo = 0.875f; yo = 0.125f;
								left = true; break;
		//7
		case STATE_CROUCHRIGHT:	xo = 0.750f; yo = 0.125f;
								break;

		case STATE_PUNCHLEFT:	xo = 1.0f; yo = 0.125f;
								left = true; break;

		case STATE_PUNCHRIGHT:	xo = 0.875f; yo = 0.125f;
								break;

		case STATE_DEAD:		xo = 0.0f + (GetFrame()*0.125f); yo = 0.5f;
								NextFrame(3); break;
	}

	yf = yo - 0.125f;
	float ix = 0.125f;
	xf = xo + ix;

	if(left) xf = xo - ix;
	DrawRect(tex_id,xo,yo,xf,yf);
}
