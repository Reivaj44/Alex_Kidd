#include "cBicho.h"
#include "cScene.h"
#include "Globals.h"

cBicho::cBicho(void)
{
	seq=0;
	delay=0;
}
cBicho::~cBicho(void){}

cBicho::cBicho(int posx,int posy,int width,int height)
{
	x = posx;
	y = posy;
	w = width;
	h = height;
}
void cBicho::SetPosition(int posx,int posy)
{
	x = posx;
	y = posy;
}
void cBicho::GetPosition(int *posx,int *posy)
{
	*posx = x;
	*posy = y;
}
void cBicho::SetTile(int tx,int ty)
{
	x = tx * TILE_SIZE;
	y = ty * TILE_SIZE;
}
void cBicho::GetTile(int *tx,int *ty)
{
	*tx = x / TILE_SIZE;
	*ty = y / TILE_SIZE;
}
void cBicho::SetWidthHeight(int width,int height)
{
	w = width;
	h = height;
}
void cBicho::GetWidthHeight(int *width,int *height)
{
	*width = w;
	*height = h;
}
bool cBicho::Collides(cRect *rc)
{
	return ((x>rc->left) && (x+w<rc->right) && (y>rc->bottom) && (y+h<rc->top));
}
bool cBicho::CollidesMapWall(int *map,bool right)
{
	int tile_x,tile_y;
	int j;
	int width_tiles,height_tiles;
	int x_aux = x;
	if(right) x_aux = x + w -1;

	tile_x = x_aux / TILE_SIZE;
	tile_y = y / TILE_SIZE;
	
	width_tiles  = w / TILE_SIZE;
	height_tiles = ( (y+h-1) / TILE_SIZE) - (y / TILE_SIZE) + 1;
	
	for(j=0;j<height_tiles;j++)
	{
		if(map[ tile_x + ((tile_y+j)*SCENE_WIDTH) ] != 0)	return true;
	}
	
	return false;
}

bool cBicho::CollidesMapFloor(int *map)
{
	int tile_x,tile_y;
	int width_tiles;
	bool on_base;
	int i;

	tile_x = x / TILE_SIZE;
	tile_y = y / TILE_SIZE;

	width_tiles = ( (x+w-1) / TILE_SIZE) - (x / TILE_SIZE) + 1;

	on_base = false;
	i=0;
	while((i<width_tiles) && !on_base)
	{
		if( (y % TILE_SIZE) == 0 )
		{
			if(map[ (tile_x + i) + ((tile_y - 1) * SCENE_WIDTH) ] != 0)
				on_base = true;
		}
		else
		{
			if(map[ (tile_x + i) + (tile_y * SCENE_WIDTH) ] != 0)
			{
				y = (tile_y + 1) * TILE_SIZE;
				on_base = true;
			}
		}
		i++;
	}
	return on_base;
}

bool cBicho::CollidesMapCeil(int *map)
{
	int tile_x,tile_y;
	int width_tiles;
	int i;
	bool collide;

	int y_aux = y+h;
	tile_x = x / TILE_SIZE;
	tile_y = y_aux / TILE_SIZE;

	width_tiles = w / TILE_SIZE;
	if( (x % TILE_SIZE) != 0) width_tiles++;

	collide = false;
	i=0;
	while((i<width_tiles) && !collide) 
	{
		if( (y_aux % TILE_SIZE) == 0) 
		{
			if(map[ (tile_x + i) + ((tile_y) * SCENE_WIDTH) ] != 0)
				collide = true;
		}
		else {
			if(map[ (tile_x + i) + ((tile_y) * SCENE_WIDTH) ] != 0)
			{
				y -= (y_aux % TILE_SIZE);
				collide = true;
			}
		}
		i++;
	}
	return collide;
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
	int screen_x,screen_y;

	screen_x = x + SCENE_Xo;
	screen_y = y + SCENE_Yo + (BLOCK_SIZE - TILE_SIZE);

	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D,tex_id);
	glBegin(GL_QUADS);	
		glTexCoord2f(xo,yo);	glVertex2i(screen_x  ,screen_y);
		glTexCoord2f(xf,yo);	glVertex2i(screen_x+w,screen_y);
		glTexCoord2f(xf,yf);	glVertex2i(screen_x+w,screen_y+h);
		glTexCoord2f(xo,yf);	glVertex2i(screen_x  ,screen_y+h);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void cBicho::NextFrame(int max)
{
	delay++;
	if(delay == FRAME_DELAY)
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