
#include "cPlayer.h"
#include "Globals.h"

cPlayer::cPlayer() 
{
	intheair = false;
	jumping = false;
	left = false;
	swimming = false;
	poisoned = false;
	retard = 0;
	punch_delay = 0;
	w = 64;
	h = 32;
	down_press = false;
	SetState(STATE_LOOKRIGHT);
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

bool cPlayer::isSwimming()
{
	return swimming;
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
		punchbox.left = x + 63 - ipunchbox.right;
		punchbox.right = x + 63 - ipunchbox.left;
	}
}

void cPlayer::ChangeBox()
{
	ipunchbox.left = 32; ipunchbox.right = 32;
	ipunchbox.bottom = 16; ipunchbox.top = 16;
	if(swimming) 
	{
		ibodybox.left = 31-8;	ibodybox.right = 32 + 8;
		ibodybox.bottom = 8;	ibodybox.top = 31 - 8;	
		if(punching)
		{
			ipunchbox.left = 32+11; ipunchbox.right = 32+18;
			ipunchbox.bottom = 8; ipunchbox.top = 15;
		}
	}
	else 
	{
		ibodybox.left = 16+10; ibodybox.right = 16+31 - 10;
		ibodybox.bottom = 5; ibodybox.top = 31 - 5;
		if(punching) 
		{
			ipunchbox.left = 16+22; ipunchbox.right = 16+30;
			ipunchbox.bottom = 12; ipunchbox.top = 19;
		}
	}
	UpdateBox();
}

void cPlayer::MoveLeft(int *map, std::vector<cBlock*> &blocks)
{
	if(state != STATE_DEAD && !poisoned)
	{
		if(state==STATE_CROUCHLEFT || state==STATE_CROUCHRIGHT)	SetState(STATE_CROUCHLEFT);
		else if (!punching || intheair || swimming)
		{
			int xaux;
			//Whats next tile?
		
			xaux = x;
			x -= step_length;
			UpdateBox();

			if(CollidesMapWall(map,false,blocks)) 
			{
				x = xaux;
				UpdateBox();
			}
			else if(state != STATE_WALKLEFT && state != STATE_SWIMLEFT)
			{
				seq = 0;
				delay = 0;
			}
			if(swimming) 
			{
				if(punching) SetState(STATE_SPUNCHLEFT);
				else SetState(STATE_SWIMLEFT);
			}
			else if(intheair && punching) SetState(STATE_PUNCHLEFT);
			else if(intheair) SetState(STATE_JUMPLEFT);
			else if(!punching) SetState(STATE_WALKLEFT);
		}
	}
}

void cPlayer::MoveRight(int *map, std::vector<cBlock*> &blocks)
{
	if(state!=STATE_DEAD && !poisoned) {
		if(state==STATE_CROUCHRIGHT || state==STATE_CROUCHLEFT) SetState(STATE_CROUCHRIGHT);
		else if (!punching || intheair || swimming)
		{
			int xaux;
			//Whats next tile?
			xaux = x;
			x += step_length;
			UpdateBox();

			
			if(CollidesMapWall(map,true,blocks)) {
				x = xaux;
				UpdateBox();
			}

			else if(state != STATE_WALKRIGHT && state!=STATE_SWIMRIGHT)
			{
				seq = 0;
				delay = 0;
			}
			if(swimming) 
			{
				if(punching) SetState(STATE_SPUNCHRIGHT);
				else SetState(STATE_SWIMRIGHT);
			}
			else if(intheair && punching) SetState(STATE_PUNCHRIGHT);
			else if(intheair) SetState(STATE_JUMPRIGHT);
			else if(!punching) SetState(STATE_WALKRIGHT);	
		}
	}
}

void cPlayer::Crouch(int *map, std::vector<cBlock*> &blocks)
{
	if(swimming && state!=STATE_DEAD && !poisoned)
	{
		y--;
		UpdateBox();
		CollidesMapFloor(map,blocks);
		down_press = true;
		if(!punching && left) SetState(STATE_SWIMLEFT); 
		else if(!punching) SetState(STATE_SWIMRIGHT);
	}
	else if(!intheair && state!=STATE_DEAD && !poisoned && !punching) {
		switch(state)
		{
			case STATE_LOOKLEFT:	SetState(STATE_CROUCHLEFT);		break;
			case STATE_LOOKRIGHT:	SetState(STATE_CROUCHRIGHT);	break;
		}
	}
}

void cPlayer::Punch(int *map)
{
	if(state!=STATE_CROUCHLEFT && state!=STATE_CROUCHRIGHT && !punching && state!=STATE_DEAD && !poisoned)
	{
		mciSendString("play SOUNDS/punch.wav", NULL, 0, NULL);
		if(swimming)
		{
			if(left) SetState(STATE_SPUNCHLEFT);
			else SetState(STATE_SPUNCHRIGHT);
		}
		else
		{
			if(!left) SetState(STATE_PUNCHRIGHT);
			else SetState(STATE_PUNCHLEFT);
		}
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
			case STATE_WALKRIGHT:	SetState(STATE_LOOKRIGHT);		break;
			case STATE_JUMPLEFT:	SetState(STATE_LOOKLEFT);		break;
			case STATE_JUMPRIGHT:	SetState(STATE_LOOKRIGHT);		break;
			case STATE_CROUCHLEFT:	SetState(STATE_LOOKLEFT);		break;
			case STATE_CROUCHRIGHT:	SetState(STATE_LOOKRIGHT);		break;
			case STATE_PUNCHLEFT:	SetState(STATE_LOOKLEFT);		break;
			case STATE_PUNCHRIGHT:	SetState(STATE_LOOKRIGHT);		break;
			case STATE_SPUNCHRIGHT:	SetState(STATE_SWIMRIGHT);		break;
			case STATE_SPUNCHLEFT:	SetState(STATE_SWIMLEFT);		break;
		}
	}
	else if(intheair && !punching && state!=STATE_DEAD) {
		switch(state) 
		{
			case STATE_WALKLEFT:		SetState(STATE_JUMPLEFT);		break;
			case STATE_WALKRIGHT:		SetState(STATE_JUMPRIGHT);		break;
			case STATE_LOOKRIGHT:		SetState(STATE_JUMPRIGHT);		break;
			case STATE_LOOKLEFT:		SetState(STATE_JUMPLEFT);		break;
			case STATE_PUNCHLEFT:		SetState(STATE_JUMPLEFT);		break;
			case STATE_PUNCHRIGHT:		SetState(STATE_JUMPRIGHT);		break;
		}
	}
		
}

void cPlayer::Jump(int *map)
{
	if(!jumping && !poisoned && !swimming)
	{
		if(!intheair && (state!=STATE_CROUCHLEFT) && (state!=STATE_CROUCHRIGHT) && !punching)
		{
			if(state==STATE_WALKLEFT || state==STATE_LOOKLEFT) SetState(STATE_JUMPLEFT);
			else if(state==STATE_WALKRIGHT || state==STATE_LOOKRIGHT) SetState(STATE_JUMPRIGHT);
			mciSendString("play SOUNDS/jump.wav", NULL, 0, NULL);
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
	poisoned = true;
}

void cPlayer::Swim()
{
	swimming = true;
	intheair = false;
	if(left) SetState(STATE_SWIMLEFT);
	else SetState(STATE_SWIMRIGHT);
}

void cPlayer::Resurrect(int tile_x, int tile_y)
{
	if(swimming) SetState(STATE_SWIMRIGHT);
	else SetState(STATE_LOOKRIGHT);
	SetTile(tile_x, tile_y);
}

void cPlayer::SetState(int s)
{
	cBicho::SetState(s);
	if(	state==STATE_LOOKLEFT	|| state==STATE_WALKLEFT	|| state==STATE_JUMPLEFT	|| 
		state==STATE_CROUCHLEFT || state==STATE_PUNCHLEFT	|| state==STATE_SWIMLEFT	|| state==STATE_SPUNCHLEFT) left = true;
	else left = false;
	if( state==STATE_SWIMLEFT	|| state==STATE_SWIMRIGHT	|| state==STATE_SPUNCHLEFT	|| state==STATE_SPUNCHRIGHT) 
	{
		frame_delay = 16;
		step_length = 1;
	}
	else
	{
		step_length = 2;
		frame_delay = 8;
	}
	if( state==STATE_PUNCHLEFT	|| state==STATE_PUNCHRIGHT	|| state==STATE_SPUNCHLEFT	|| state==STATE_SPUNCHRIGHT) punching = true;
	else punching = false;
	ChangeBox();
}

void cPlayer::Logic(int *map, std::vector<cMonster*> &monsters, std::vector<cBlock*> &blocks)
{
	if(state==STATE_DEAD) 
	{
		y += 1;
		UpdateBox();
	}

	else 
	{
		if(poisoned)
		{
			if(!swimming) Stop();
			if(swimming || CollidesMapFloor(map,blocks)) {
				if(retard%4==0) x--;
				else if(retard%2==0) x++;
				retard++;
				if(retard > 50)
				{
					poisoned = false;
					retard = 0;
				}
			}
		}
		if(punching) 
		{
			for(unsigned int i = 0; i < monsters.size(); i++) 
				if(!monsters[i]->isDead() && monsters[i]->CollidesBox(punchbox)) monsters[i]->Die();
			for(unsigned int i = 0; i < blocks.size(); i++) 
				if(blocks[i]->isCollisionable() && blocks[i]->CollidesBox(punchbox)) blocks[i]->Destroy();

			punch_delay++;
			if(punch_delay >= frame_delay && (!swimming || !poisoned) )
			{
				punch_delay = 0;
				punching = false;
			}
		}
		if(jumping && !swimming)
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
		else if(!jumping && !swimming)
		{
			//Over floor?
			if(!CollidesMapFloor(map,blocks)) {
				y -= (1.5*step_length);
				UpdateBox();
				intheair=true;
			}

			else intheair=false;
		}
		if(swimming && !poisoned && delay%2==0) 
		{
			if(!down_press)
			{
				y++;
				UpdateBox();
				CollidesMapCeil(map,blocks);
			}
			down_press = false;
		}
	}
}

void cPlayer::Draw(int tex_id)
{	
	float xo,yo,xf,yf;
	switch(GetState())
	{
		//5
		case STATE_LOOKLEFT:	xo = 0.25f;	yo = 0.25f; 
								break;
		//5
		case STATE_LOOKRIGHT:	xo = 0.0f;	yo = 0.25f;
								break;
		//4..1
		case STATE_WALKLEFT:	xo = 0.25f + (GetFrame()*0.25f); yo = 0.125;
								NextFrame(4);
								break;
		//1..4
		case STATE_WALKRIGHT:	xo = 0.0f + (GetFrame()*0.25f); yo = 0.125f;
								NextFrame(4);
								break;
		//6
		case STATE_JUMPLEFT:	xo = 0.5f; yo = 0.25f;
								break;
		//6
		case STATE_JUMPRIGHT:	xo = 0.25f; yo = 0.25;
								break;
		//7
		case STATE_CROUCHLEFT:	xo = 0.75f; yo = 0.25f;
								break;
		//7
		case STATE_CROUCHRIGHT:	xo = 0.5f; yo = 0.25f;
								break;

		case STATE_PUNCHLEFT:	xo = 1.0f; yo = 0.25f;
								break;

		case STATE_PUNCHRIGHT:	xo = 0.75f; yo = 0.25f;
								break;

		case STATE_SWIMLEFT:	xo = 0.25f + (GetFrame()*0.25f); yo = 0.375;
								NextFrame(2); break;

		case STATE_SWIMRIGHT:	xo = 0.0f + (GetFrame()*0.25f);	yo = 0.375;
								NextFrame(2); break;

		case STATE_SPUNCHLEFT:	xo = 0.75f; yo = 0.375;
								break;

		case STATE_SPUNCHRIGHT:	xo = 0.5f;	yo = 0.375f;
								break;

		case STATE_DEAD:		xo = 0.0f + (GetFrame()*0.25f); yo = 0.875f;
								NextFrame(3); break;
	}

	yf = yo - 0.125f;
	float ix = 0.25f;
	xf = xo + ix;

	if(left) xf = xo - ix;
	DrawRect(tex_id,xo,yo,xf,yf);
}
