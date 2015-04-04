#pragma once

#include "cTexture.h"

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

private:
	int map[SCENE_WIDTH * SCENE_HEIGHT];	//scene
	int id_DL;								//actual level display list
};
