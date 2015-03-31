#pragma once

#include "cScene.h"
#include "cPlayer.h"
#include "cMonster.h"
#include "cBlock.h"
#include "cData.h"

#define GAME_WIDTH	640
#define GAME_HEIGHT 480
#define CAM_STEP 0.1

class cGame
{
public:
	cGame(void);
	virtual ~cGame(void);

	bool Init();
	bool Loop();
	void Finalize();

	//Input
	void ReadKeyboard(unsigned char key, int x, int y, bool press);
	void ReadMouse(int button, int state, int x, int y);
	//Process
	bool Process();
	//Output
	void Render();

private:
	unsigned char keys[256];
	bool jump_key;
	bool punch_key;
	float cam_x, cam_y, cam_w, cam_h;
	float mov_x, mov_y;
	cScene Scene;
	cPlayer Player;
	std::vector<cMonster*> monsters;
	std::vector<cBlock*> blocks;
	cData Data;
};
