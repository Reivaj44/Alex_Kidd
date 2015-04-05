#pragma once

#include "cScene.h"
#include "cPlayer.h"
#include "cMonster.h"
#include "cBlock.h"
#include "cData.h"
#include "cTexture.h"

#define GAME_WIDTH	640
#define GAME_HEIGHT 480
#define CAM_WIDTH 256
#define CAM_HEIGHT 192
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
	//Load
	bool InitIntro();
	bool InitInstrucc();
	bool InitCredits();
	bool InitLevel1();

private:
	unsigned char keys[256];
	int rectangle;
	bool jump_key;
	bool punch_key;
	bool reappears;
	int delay;
	cRect cam;
	cScene Scene;
	cPlayer Player;
	int lifes;
	bool ring;
	int money;
	int score;
	int stage;
	int option;
	std::vector<cMonster*> monsters;
	std::vector<cBlock*> blocks;
	cData Data;
};
