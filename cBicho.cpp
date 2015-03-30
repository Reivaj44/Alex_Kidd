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
	UpdateBox();
}
void cBicho::GetPosition(int &posx,int &posy)
{
	posx = x;
	posy = y;
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

bool cBicho::Collides(cRect *rc)
{
	return ((x>rc->left) && (x+w<rc->right) && (y>rc->bottom) && (y+h<rc->top));
}
bool cBicho::CollidesMapWall(int *map,bool right)
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

	tile_x = bodybox.left / TILE_SIZE;
	tile_y = bodybox.bottom / TILE_SIZE;

	width_tiles = ( (bodybox.right + 1) / TILE_SIZE ) - ( bodybox.left / TILE_SIZE ) + 1;

	on_base = false;
	i=0;
	while((i<width_tiles) && !on_base)
	{
		if( (bodybox.bottom % TILE_SIZE) == 0 )
		{
			if(map[ (tile_x + i) + ((tile_y - 1) * SCENE_WIDTH) ] != 0)
				on_base = true;
		}
		else
		{
			if(map[ (tile_x + i) + (tile_y * SCENE_WIDTH) ] != 0)
			{
				y += TILE_SIZE - (bodybox.bottom % TILE_SIZE);
				UpdateBox();
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

	int y_aux = bodybox.top + 1;
	tile_x = bodybox.left / TILE_SIZE;
	tile_y = y_aux / TILE_SIZE;

	width_tiles = ( (bodybox.right+1) / TILE_SIZE) - (bodybox.left / TILE_SIZE) + 1;

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
			int k = map[ (tile_x + i) + ((tile_y) * SCENE_WIDTH) ];
			if(map[ (tile_x + i) + ((tile_y) * SCENE_WIDTH) ] != 0)
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