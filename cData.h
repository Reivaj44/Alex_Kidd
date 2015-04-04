#pragma once

#include "cTexture.h"
#include "Globals.h"

//Image array size
#define NUM_IMG		7

//Image identifiers
#define IMG_INTRO	0
#define IMG_INSTRUC	1
#define IMG_CREDITS	2
#define IMG_TILES	3
#define IMG_PLAYER	4
#define IMG_ENEMY	5
#define IMG_BLOCKS	6
/*
#define IMG_ENEMY1	2
#define IMG_ENEMY2	3
#define IMG_SHOOT	4
...
*/

class cData
{
public:
	cData(void);
	~cData(void);

	int  GetID(int img);
	void GetSize(int img,int *w,int *h);
	bool LoadImage(int img,char *filename,int type = GL_RGBA);

private:
	cTexture texture[NUM_IMG];
};
