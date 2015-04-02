#pragma once

#include "cScene.h"
#include "cPlayer.h"
#include "cMonster.h"
#include "cBlock.h"
#include "cData.h"

#define GAME_WIDTH	640
#define GAME_HEIGHT 480
#define CAM_STEP 0.1
#define CAM_WIDTH 256
#define CAM_HEIGHT 192

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
	float cam_x, cam_y;
	cScene Scene;
	cPlayer Player;
	int lifes;
	int ring;
	int money;
	int score;
	std::vector<cMonster*> monsters;
	std::vector<cBlock*> blocks;
	cData Data;
};
