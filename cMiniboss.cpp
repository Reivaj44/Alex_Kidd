
#include "cMiniboss.h"
#include "cScene.h"


cMiniboss::cMiniboss(void)
{
	state = STATE_UP;
	ibodybox.left = 6; ibodybox.right = 31-5;
	ibodybox.bottom = 0; ibodybox.top = 31-0;
	yact = 0;
	lifes = 2;
	touched = false;
	booblechange = false;
	for(unsigned int i=0; i<5; i++)
	{
		cBooble* booble = new cBooble();
		boobles.push_back(booble);
	}
	boobles[0]->dir_x = -0.75f;
	boobles[0]->dir_y = 0.4;
	boobles[1]->dir_x = 0.75;
	boobles[1]->dir_y = 0.4;
	boobles[2]->dir_x = 0.50;
	boobles[2]->dir_y = 0.4;
	boobles[3]->dir_x = -0.75;
	boobles[3]->dir_y = 0.75;
	boobles[4]->dir_x = 0.75;
	boobles[4]->dir_y = 0.75;

}


cMiniboss::~cMiniboss(void)
{
	for(unsigned int i=0; i<5; i++)
	{
		delete boobles[i];
	}
}

void cMiniboss::Die()
{
	if(!touched)
	{
		lifes--;
		if(lifes==0) cMonster::Die();
		touched = true;
	}
}

void cMiniboss::Draw(int tex_id)
{	
	if(state==STATE_EXPLODE) cMonster::Draw(tex_id);
	else
	{
		float xo,yo,xf,yf;
		if(state==STATE_UP || state==STATE_DOWN) 
		{
			xo = 0.0f + (GetFrame()*0.125f);
			yo = 0.5f;
			NextFrame(2);
		}
		xf = xo + 0.125;
		yf = yo - 0.125f;
		DrawBoobles(tex_id);
		DrawRect(tex_id,xo,yo,xf,yf);
	}
}

void cMiniboss::Logic(int *map, cPlayer &player, std::vector<cBlock*> &blocks, const cRect &rectangle, int level_width) {
	if(state!=STATE_EXPLODE) {
		if(state == STATE_UP) 
		{
			if( (yact) < 3*TILE_SIZE) 
			{
				y+=step_length;
				yact+=step_length;
				UpdateBox();
			}
			else state = STATE_DOWN;
		}
		else
		{
			if( (yact) > 0) 
			{
				y-=step_length;
				yact-=step_length;
				UpdateBox();
			}
			else state = STATE_UP;
		}
		if(yact==0)
		{
			for(unsigned int i=0; i<boobles.size(); i++)
			{
				cRect &b_bodybox = boobles[i]->bodybox;
				b_bodybox.bottom = bodybox.top - 8;
				b_bodybox.left = bodybox.left;
				b_bodybox.top = bodybox.top;
				b_bodybox.right = bodybox.left + 8;
				boobles[i]->inc_y = 0;
			}
			if(booblechange)
			{
				booblechange = false;
				if(boobles[4]->dir_x == 0.75)
				{
					boobles[4]->dir_x = -0.5;
					boobles[4]->dir_y = 0.4;
				}
				else
				{
					boobles[4]->dir_x = 0.75;
					boobles[4]->dir_y = 0.75;
				}
			}
		}
		else
		{
			booblechange = true;
			for(unsigned int i=0; i<boobles.size(); i++)
			{
				cRect &b_bodybox = boobles[i]->bodybox;
				b_bodybox.bottom += boobles[i]->dir_y;
				b_bodybox.left += boobles[i]->dir_x;
				b_bodybox.top += boobles[i]->dir_y;
				b_bodybox.right += boobles[i]->dir_x;
				boobles[i]->inc_y+=boobles[i]->dir_y;
			}
		}
		if(touched && !CollidesBox(player.GetHitBox())) touched = false;
	}
	if(state!=STATE_EXPLODE && CollidesBoobles(player.GetBodyBox()) && !player.isDead()) player.Die();
	else cMonster::Logic(map,player, blocks, rectangle, level_width);
}

bool cMiniboss::CollidesBoobles(cRect &playerbox)
{
	for(unsigned int i = 0; i<boobles.size(); i++)
	{	if(boobles[i]->inc_y<7*TILE_SIZE) 
		{
			cRect b_box = boobles[i]->bodybox;
			if ((b_box.left) <= (playerbox.right) &&
			(b_box.right) >= (playerbox.left) &&
			(b_box.bottom) <= (playerbox.top) &&
			(b_box.top) >= (playerbox.bottom)) 
				return true;
		}
	}
	return false;
}

void cMiniboss::DrawBoobles(int tex_id)
{
	float xo = 0.250f+0.0625f*GetFrame();
	float xf = xo + 0.03125f;
	float yo = 0.5f;
	float yf = yo - 0.03125f;
	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D,tex_id);
	for(unsigned int i=0; i<boobles.size(); i++)
	{
		if(boobles[i]->inc_y<7*TILE_SIZE) 
		{
			cRect b_bodybox = boobles[i]->bodybox;
			glBegin(GL_QUADS);	
				glTexCoord2f(xo,yo);	glVertex2i(b_bodybox.left , b_bodybox.bottom);
				glTexCoord2f(xf,yo);	glVertex2i(b_bodybox.right, b_bodybox.bottom);
				glTexCoord2f(xf,yf);	glVertex2i(b_bodybox.right, b_bodybox.top);
				glTexCoord2f(xo,yf);	glVertex2i(b_bodybox.left , b_bodybox.top);
			glEnd();
		}
	}
	glDisable(GL_TEXTURE_2D);
}
