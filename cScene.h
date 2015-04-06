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
	int GetNumRects();
	cRect* GetRectangles(int i);
	int GetIsWater(int i);
	void GetPlayerInitPosition(int* x, int* y, int num_rect);

private:
	int map[SCENE_WIDTH * SCENE_HEIGHT];	//scene
	std::vector<cRect*> rects;				//rectangles dimensions
	std::vector<int> isWater;				//tells if a rectangle is water
	int id_DL;								//actual level display list
	int bg_DL;								//actual background display list
	std::vector<int> player_position;		//initial player position
};
