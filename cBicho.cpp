#pragma once
#include "cBicho.h"
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

bool cBicho::Appears(const cRect &cam) {
	bool incamera = CollidesBox(cam);
	return (state!=STATE_DISAPPEARED && incamera);
}

bool cBicho::CollidesMapWall(int *map,bool right, std::vector<cBlock*> &blocks, const cRect &rectangle,int level_width)
{
	int tile_x,tile_y;
	int j;
	int width_tiles,height_tiles;

	int x_aux = bodybox.left;
	if(right) x_aux = bodybox.right;

	tile_x = x_aux / TILE_SIZE;
	tile_y = bodybox.bottom / TILE_SIZE;
	
	height_tiles = int( bodybox.top / TILE_SIZE) - int(bodybox.bottom / TILE_SIZE) + 1;
	
	if(CollidesBorder(bodybox,rectangle)) return true;

	for(unsigned int i = 0; i < blocks.size(); i++) 
		if(blocks[i]->isCollisionable() && blocks[i]->CollidesBox(bodybox)) return true;

	for(j=0;j<height_tiles;j++)
	{
		if((map[ tile_x + ((tile_y+j)*level_width) ] != 0))	return true;
	}
	
	return false;
}

bool cBicho::CollidesMapFloor(int *map, std::vector<cBlock*> &blocks, int level_width)
{
	int tile_x,tile_y;
	int width_tiles;
	unsigned int i;

	tile_x = bodybox.left / TILE_SIZE;
	tile_y = bodybox.bottom / TILE_SIZE;

	width_tiles = int( (bodybox.right) / TILE_SIZE ) - int( bodybox.left / TILE_SIZE ) + 1;

	bool collides=false;
	bool collides_down=false;
	cRect bodyboxdown=bodybox;
	bodyboxdown.bottom-=1;
	i=0;
	while(i<blocks.size() && !collides && !collides_down)
	{
		if(blocks[i]->isCollisionable() && blocks[i]->CollidesBox(bodybox)) collides=true;
		if(blocks[i]->isCollisionable() && blocks[i]->CollidesBox(bodyboxdown)) collides_down=true;
		i++;
	}
	i=0;
	while((i<width_tiles) && !collides && !collides_down)
	{
		if( (int(bodybox.bottom) % TILE_SIZE) == 0 && (map[ (tile_x + i) + ((tile_y - 1) * level_width) ] != 0) )
			collides_down = true;
		else if((map[ (tile_x + i) + (tile_y * level_width) ] != 0))
			collides = true;
		i++;
	}
	if(collides)
	{
			y += TILE_SIZE - (int(bodybox.bottom) % TILE_SIZE);
			UpdateBox();
	}

	return (collides || collides_down);
}

bool cBicho::CollidesMapCeil(int *map, std::vector<cBlock*> &blocks, const cRect &rectangle, int level_width)
{
	int tile_x,tile_y;
	int width_tiles;
	bool on_base;
	unsigned int i;

	tile_x = bodybox.left / TILE_SIZE;
	tile_y = bodybox.top / TILE_SIZE;

	width_tiles = int( (bodybox.right) / TILE_SIZE ) - int( bodybox.left / TILE_SIZE ) + 1;

	bool collides=false;
	bool collides_up=false;
	cRect bodyboxup=bodybox;
	bodyboxup.top+=1;

	if(CollidesBorder(bodybox, rectangle)) collides=true;
	else if(CollidesBorder(bodyboxup, rectangle)) collides_up=true;

	i=0;
	while(i<blocks.size() && !collides && !collides_up)
	{
		if(blocks[i]->isCollisionable() && blocks[i]->CollidesBox(bodybox)) collides=true;
		if(blocks[i]->isCollisionable() && blocks[i]->CollidesBox(bodyboxup)) collides_up=true;
		i++;
	}
	i=0;
	while((i<width_tiles) && !collides && !collides_up)
	{
		if( (int(bodybox.top) % TILE_SIZE) == 15 && (map[ (tile_x + i) + ((tile_y + 1) * level_width) ] != 0) )
			collides_up = true;
		else if((map[ (tile_x + i) + (tile_y * level_width) ] != 0))
			collides = true;
		i++;
	}
	if(collides)
	{
			y -= (int(bodybox.top) % TILE_SIZE) + 1;
			UpdateBox();
	}

	return (collides || collides_up);
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

bool cBicho::BoxInsideBox(const cRect& BigBox, const cRect& SmallBox)
{
	return (SmallBox.bottom >= BigBox.bottom	&& 
			SmallBox.left	>= BigBox.left		&&
			SmallBox.top	<= BigBox.top		&&
			SmallBox.right	<= BigBox.right		);
}

bool cBicho::CollidesBorder(const cRect &box, const cRect &border)
{
	return ((box.left<(border.left)) || (box.right>(border.right)) || (box.bottom<(border.bottom)) || (box.top>(border.top)));
}

bool cBicho::InBox(const cRect &box)
{
	return (bodybox.top		>= box.bottom	&& 
			bodybox.left	>= box.left		&&
			bodybox.top		<= box.top		&&
			bodybox.left	<= box.right	);
}
