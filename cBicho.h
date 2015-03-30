#pragma once

#include "cTexture.h"
#include "Globals.h"

#define FRAME_DELAY		8
#define STEP_LENGTH		2


class cRect
{
public:
	int left,top,
		right,bottom;
};

class cBicho
{
public:
	cBicho(void);
	cBicho(int x,int y,int w,int h);
	~cBicho(void);

	void SetPosition(int x,int y);
	void GetPosition(int &x,int &y);
	void SetTile(int tx,int ty);
	void GetTile(int &tx,int &ty);
	void SetWidthHeight(int w,int h);
	void GetWidthHeight(int &w,int &h);
	cRect GetBodyBox();

	bool Collides(cRect *rc);
	bool CollidesMapWall(int *map,bool right);
	bool CollidesMapFloor(int *map);
	bool CollidesMapCeil(int *map);
	bool CollidesBox(const cRect &box);

	void GetArea(cRect *rc);
	void DrawRect(int tex_id,float xo,float yo,float xf,float yf);

	int  GetState();
	void SetState(int s);

	void NextFrame(int max);
	int  GetFrame();

	virtual void Draw(int tex_id) = 0;
	
protected:
	int x,y;
	int w,h;
	int state;
	cRect bodybox;
	cRect ibodybox;
	int seq,delay;

	virtual void UpdateBox();
};
