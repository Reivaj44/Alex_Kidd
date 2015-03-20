
#include "cPlayer.h"
#include "Globals.h"
#include "cScene.h"

cPlayer::cPlayer() 
{
	intheair = false;
	jumping = false;	
}
cPlayer::~cPlayer(){}

void cPlayer::MoveLeft(int *map)
{
	if(state==STATE_CROUCHLEFT || state==STATE_CROUCHRIGHT) state = STATE_CROUCHLEFT;
	else 
	{
		int xaux;
		//Whats next tile?
		if( (x % TILE_SIZE) == 0)
		{
			xaux = x;
			x -= STEP_LENGTH;

			if(CollidesMapWall(map,false))
				x = xaux;
		}
		//Advance, no problem
		else
		{
			x -= STEP_LENGTH;
			if(state != STATE_WALKLEFT)
			{
				seq = 0;
				delay = 0;
			}
		}
		if(intheair) state = STATE_JUMPLEFT;
		else state = STATE_WALKLEFT;
	}	
}

void cPlayer::MoveRight(int *map)
{
	if(state==STATE_CROUCHLEFT || state==STATE_CROUCHRIGHT) state = STATE_CROUCHRIGHT;
	else {
		int xaux;

		//Whats next tile?
		if( (x % TILE_SIZE) == 0)
		{
			xaux = x;
			x += STEP_LENGTH;

			if(CollidesMapWall(map,true))
				x = xaux;
		}
		//Advance, no problem
		else
		{
			x += STEP_LENGTH;

			if(state != STATE_WALKRIGHT)
			{
				seq = 0;
				delay = 0;
			}
		}
		if(intheair) state = STATE_JUMPRIGHT;
		else state = STATE_WALKRIGHT;		
	}
}

void cPlayer::Crouch(int *map)
{
	if(!intheair) {
		if(CollidesMapWall(map,true) && state==STATE_WALKRIGHT) state = STATE_CROUCHRIGHT;
		else if(CollidesMapWall(map,false) && state==STATE_WALKLEFT) state = STATE_CROUCHLEFT;
		else
		{
			switch(state)
			{
				case STATE_LOOKLEFT:	state = STATE_CROUCHLEFT;		break;
				case STATE_LOOKRIGHT:	state = STATE_CROUCHRIGHT;		break;
			}
		}
	}
}

void cPlayer::Punch(int *map)
{
	if(state!=STATE_CROUCHLEFT && state!=STATE_CROUCHRIGHT)
	{
		if(state==STATE_WALKRIGHT || state==STATE_LOOKRIGHT || state==STATE_JUMPRIGHT) state = STATE_PUNCHRIGHT;
		else state = STATE_PUNCHLEFT;
	}
}

void cPlayer::Stop()
{
	if(!intheair){
		switch(state)
		{
			case STATE_WALKLEFT:	state = STATE_LOOKLEFT;		break;
			case STATE_WALKRIGHT:	state = STATE_LOOKRIGHT;	break;
			case STATE_JUMPLEFT:	state = STATE_LOOKLEFT;		break;
			case STATE_JUMPRIGHT:	state = STATE_LOOKRIGHT;	break;
			case STATE_CROUCHLEFT:	state = STATE_LOOKLEFT;		break;
			case STATE_CROUCHRIGHT:	state = STATE_LOOKRIGHT;	break;
			case STATE_PUNCHLEFT:	state = STATE_LOOKLEFT;		break;
			case STATE_PUNCHRIGHT:	state = STATE_LOOKRIGHT;	break;
		}
	}
	else {
		switch(state) 
		{
			case STATE_WALKLEFT:		state = STATE_JUMPLEFT;		break;
			case STATE_WALKRIGHT:		state = STATE_JUMPRIGHT;	break;
			case STATE_LOOKRIGHT:		state = STATE_JUMPRIGHT;	break;
			case STATE_LOOKLEFT:		state = STATE_JUMPLEFT;		break;
			case STATE_PUNCHLEFT:		state = STATE_JUMPLEFT;		break;
			case STATE_PUNCHRIGHT:		state = STATE_JUMPRIGHT;	break;
		}
	}
		
}

void cPlayer::Jump(int *map)
{
	if(!jumping)
	{
		if(CollidesMapFloor(map))
		{
			if(state==STATE_WALKLEFT || state==STATE_LOOKLEFT) state = STATE_JUMPLEFT;
			else if(state==STATE_WALKRIGHT || state==STATE_LOOKRIGHT)	state = STATE_JUMPRIGHT;
			jumping = true;
			intheair = true;
			jump_alfa = 0;
			jump_y = y;
		}
	}
}

void cPlayer::Logic(int *map)
{
	float alfa;

	if(jumping)
	{
		jump_alfa += JUMP_STEP;
		
		if(jump_alfa == 180)
		{
			jumping = false;
			y = jump_y;
		}
		else 
		{
			alfa = ((float)jump_alfa) * 0.017453f;
			y = jump_y + (int)( ((float)JUMP_HEIGHT) * sin(alfa) );
			
			if(jump_alfa <= 90 && CollidesMapCeil(map))
				jumping = false;

			if(jump_alfa > 90)
			{
				//Over floor?
				jumping=!CollidesMapFloor(map);
			}
		}
	}
	else
	{
		//Over floor?
		if(!CollidesMapFloor(map)) {
			y -= (2*STEP_LENGTH);
			intheair=true;
		}
		else intheair=false;
	}
}

void cPlayer::Draw(int tex_id)
{	
	float xo,yo,xf,yf;
	bool left = false;
	bool punch = false;
	switch(GetState())
	{
		//5
		case STATE_LOOKLEFT:	xo = 0.125f;	yo = 0.875f;
								left = true; break;
		//5
		case STATE_LOOKRIGHT:	xo = 0.0f;	yo = 0.875f;
								break;
		//4..1
		case STATE_WALKLEFT:	xo = 0.125f + (GetFrame()*0.125f); yo = 0.375f;
								NextFrame(4);
								left = true; break;
		//1..4
		case STATE_WALKRIGHT:	xo = 0.0f + (GetFrame()*0.125f); yo = 0.375f;
								NextFrame(4);
								break;
		//6
		case STATE_JUMPLEFT:	xo = 0.250f; yo = 0.875f;
								left = true; break;
		//6
		case STATE_JUMPRIGHT:	xo = 0.125f; yo = 0.875f;
								break;
		//7
		case STATE_CROUCHLEFT:	xo = 0.375f; yo = 0.875f;
								left = true; break;
		//7
		case STATE_CROUCHRIGHT:	xo = 0.250f; yo = 0.875f;
								break;

		case STATE_PUNCHLEFT:	xo = 0.5625f; yo = 0.875f;
								left = true; punch = true; break;

		case STATE_PUNCHRIGHT:	xo = 0.375f; yo = 0.875f;
								left = false; punch = true; break;

	}
	float ix = 0.125f;
	if(punch) ix = 0.1875;

	if(left) {
		xf = xo - ix;
		yf = yo - 0.375f;
	}
	else {
		xf = xo + ix;
		yf = yo - 0.375f;
	}
	DrawRect(tex_id,xo,yo,xf,yf);
}
