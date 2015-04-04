#pragma once

#include "cTexture.h"
#include "cBicho.h"

#define SCENE_WIDTH		48
#define SCENE_HEIGHT	120

#define FILENAME		"level"
#define FILENAME_EXT	".txt"

#define TILE_SIZE		16

class cScene
{
public:
	cScene(void);
	virtual ~cScene(void);

	bool LoadLevel(int level);
	void Draw(int tex_id);
	int *GetMap();

	std::vector<cRect*> rects;				//rectangles dimensions
	std::vector<int> isWater;

private:
	int map[SCENE_WIDTH * SCENE_HEIGHT];	//scene
					//tells if a rectangle is water
	int id_DL;								//actual level display list
	int bg_DL;								//actual background display list
};
