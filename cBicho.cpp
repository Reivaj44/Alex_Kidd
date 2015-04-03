#pragma once
#include "cBicho.h"
#include "cScene.h"
#include "Globals.h"
#include "cBlock.h"
#include "cData.h"

cBicho::cBicho(void)
{
	seq=0;
	delay=0;
	frame_delay=8;
	step_length=2;
}
cBicho::~cBicho(void){}

cBicho::cBicho(int posx,int posy,int width,int height)
{
	x = posx - ibodybox.left;
	y = posy - ibodybox.bottom;
	w = width;
	h = height;
}
void cBicho::SetPosition(int posx,int posy)
{
	x = posx - ibodybox.left;
	y = posy - ibodybox.bottom;
	UpdateBox();
}
void cBicho::GetPosition(int &posx,int &posy)
{
	posx = x + ibodybox.left;
	posy = y + ibodybox.bottom;
}
void cBicho::SetTile(int tx,int ty)
{
	x = tx * TILE_SIZE - ibodybox.left;
	y = ty * TILE_SIZE - ibodybox.bottom;
	UpdateBox();
}
void cBicho::GetTile(int &tx,int &ty)
{
	tx = x / TILE_SIZE + ibodybox.left;
	ty = y / TILE_SIZE + ibodybox.bottom;
}
void cBicho::SetWidthHeight(int width,int height)
{
	w = width;
	h = height;
}

void cBicho::GetWidthHeight(int &width,int &height)
{
	width = w;
	height = h;
}

cRect cBicho::GetBodyBox()
{
	return bodybox;
}


void cBicho::UpdateBox() 
{
	bodybox.left = x + ibodybox.left;
	bodybox.right = x + ibodybox.right;
	bodybox.bottom = y + ibodybox.bottom;
	bodybox.top = y + ibodybox.top;
}

bool cBicho::Appears(int cam_x, int cam_y) {
	cRect boxcamera;
	boxcamera.bottom = cam_y;
	boxcamera.left = cam_x;
	boxcamera.top = cam_y + CAM_HEIGHT;
	boxcamera.right = cam_x + CAM_WIDTH;
	bool incamera = CollidesBox(boxcamera);
	return (state!=STATE_DISAPPEARED && incamera);
}

bool cBicho::Collides(cRect *rc)
{
	return ((x>rc->left) && (x+w<rc->right) && (y>rc->bottom) && (y+h<rc->top));
}
bool cBicho::CollidesMapWall(int *map,bool right, std::vector<cBlock*> &blocks)
{
	int tile_x,tile_y;
	int j;
	int width_tiles,height_tiles;

	int x_aux = bodybox.left;
	if(right) x_aux = bodybox.right;

	tile_x = x_aux / TILE_SIZE;
	tile_y = bodybox.bottom / TILE_SIZE;
	
	width_tiles  = (bodybox.left - bodybox.right + 1) / TILE_SIZE;
	height_tiles = ( bodybox.top / TILE_SIZE) - (bodybox.bottom / TILE_SIZE) + 1;

	bool collideswithblock=false;
	for(unsigned int i = 0; i < blocks.size(); i++) 
		if(blocks[i]->isCollisionable() && blocks[i]->CollidesBox(bodybox)) collideswithblock=true;

	for(j=0;j<height_tiles;j++)
	{
		if((map[ tile_x + ((tile_y+j)*SCENE_WIDTH) ] != 0) || collideswithblock)	return true;
	}
	
	return false;
}

bool cBicho::CollidesMapFloor(int *map, std::vector<cBlock*> &blocks)
{
	int tile_x,tile_y;
	int width_tiles;
	bool on_base;
	int i;

	tile_x = bodybox.left / TILE_SIZE;
	tile_y = bodybox.bottom / TILE_SIZE;

	width_tiles = ( (bodybox.right) / TILE_SIZE ) - ( bodybox.left / TILE_SIZE ) + 1;

	bool collideswithblock=false;
	bool collideswithblockdown=false;
	cRect bodyboxdown=bodybox;
	bodyboxdown.bottom-=1;
	for(unsigned int i = 0; i < blocks.size(); i++) 
	{
		if(blocks[i]->isCollisionable() && blocks[i]->CollidesBox(bodybox)) collideswithblock=true;
		if(blocks[i]->isCollisionable() && blocks[i]->CollidesBox(bodyboxdown)) collideswithblockdown=true;
	}
	on_base = false;
	i=0;
	while((i<width_tiles) && !on_base)
	{
		if( (int(bodybox.bottom) % TILE_SIZE) == 0 )
		{
			if((map[ (tile_x + i) + ((tile_y - 1) * SCENE_WIDTH) ] != 0) || collideswithblockdown)
				on_base = true;
		}
		else
		{
			if((map[ (tile_x + i) + (tile_y * SCENE_WIDTH) ] != 0) || collideswithblock)
			{
				y += TILE_SIZE - (int(bodybox.bottom) % TILE_SIZE);
				UpdateBox();
				on_base = true;
			}
		}
		i++;
	}
	return on_base;
}

bool cBicho::CollidesMapCeil(int *map, std::vector<cBlock*> &blocks)
{
	int tile_x,tile_y;
	int width_tiles;
	int i;
	bool collide;

	int y_aux = bodybox.top+1;
	tile_x = bodybox.left / TILE_SIZE;
	tile_y = y_aux / TILE_SIZE;

	width_tiles = ( (bodybox.right) / TILE_SIZE) - (bodybox.left / TILE_SIZE) + 1;

	bool collideswithblock=false;
	for(unsigned int i = 0; i < blocks.size(); i++) 
		if(blocks[i]->isCollisionable() && blocks[i]->CollidesBox(bodybox)) collideswithblock=true;

	collide = false;
	i=0;
	while((i<width_tiles) && !collide) 
	{
		if( (y_aux % TILE_SIZE) == 0) 
		{
			if((map[ (tile_x + i) + ((tile_y) * SCENE_WIDTH) ] != 0) || collideswithblock)
				collide = true;
		}
		else {
			if((map[ (tile_x + i) + ((tile_y) * SCENE_WIDTH) ] != 0) || collideswithblock)
			{
				y -= (y_aux % TILE_SIZE);
				UpdateBox();
				collide = true;
			}
		}
		i++;
	}
	return collide;
}

bool cBicho::CollidesBox(const cRect &box)
{
	if ((bodybox.left) <= (box.right) &&
		(bodybox.right) >= (box.left) &&
		(bodybox.bottom) <= (box.top) &&
		(bodybox.top) >= (box.bottom)) 
			return true;
	else return false;
}

void cBicho::GetArea(cRect *rc)
{
	rc->left   = x;
	rc->right  = x+w;
	rc->bottom = y;
	rc->top    = y+h;
}

void cBicho::DrawRect(int tex_id,float xo,float yo,float xf,float yf)
{
	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D,tex_id);
	glBegin(GL_QUADS);	
		glTexCoord2f(xo,yo);	glVertex2i(x  , y);
		glTexCoord2f(xf,yo);	glVertex2i(x+w, y);
		glTexCoord2f(xf,yf);	glVertex2i(x+w, y+h);
		glTexCoord2f(xo,yf);	glVertex2i(x  , y+h);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void cBicho::NextFrame(int max)
{
	delay++;
	if(delay == frame_delay)
	{
		seq++;
		seq%=max;
		delay = 0;
	}
}

int cBicho::GetFrame()
{
	return seq;
}

int cBicho::GetState()
{
	return state;
}

void cBicho::SetState(int s)
{
	state = s;
}