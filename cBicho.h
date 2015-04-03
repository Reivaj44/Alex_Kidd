#pragma once

#include "cTexture.h"
#include "Globals.h"

#define STATE_DISAPPEARED -1

class cBlock;

class cRect
{
public:
	float left,top,
		right,bottom;
};

class cBicho
{
public:
	cBicho(void);
	cBicho(int x,int y,int w,int h);
	virtual ~cBicho(void);

	void SetPosition(int x,int y);
	void GetPosition(int &x,int &y);
	void SetTile(int tx,int ty);
	void GetTile(int &tx,int &ty);
	void SetWidthHeight(int w,int h);
	void GetWidthHeight(int &w,int &h);
	cRect GetBodyBox();

	virtual bool Appears(int cam_x, int cam_y);
	bool Collides(cRect *rc);
	bool CollidesMapWall(int *map,bool right, std::vector<cBlock*> &blocks);
	bool CollidesMapFloor(int *map, std::vector<cBlock*> &blocks);
	bool CollidesMapCeil(int *map, std::vector<cBlock*> &blocks);
	bool CollidesBox(const cRect &box);

	void GetArea(cRect *rc);
	void DrawRect(int tex_id,float xo,float yo,float xf,float yf);

	int  GetState();
	virtual void SetState(int s);

	void NextFrame(int max);
	int  GetFrame();

	virtual void Draw(int tex_id) = 0;
	
protected:
	float x,y;
	int w,h;
	int state;
	cRect bodybox;
	cRect ibodybox;
	int seq;
	int frame_delay;
	int delay;
	float step_length;

	virtual void UpdateBox();
};
