#pragma once

#include "cScene.h"
#include "cPlayer.h"
#include "cMonster.h"
#include "cBlock.h"
#include "cData.h"
#include "cTexture.h"

#define GAME_WIDTH	640
#define GAME_HEIGHT 480
#define CAM_WIDTH 255
#define CAM_HEIGHT 191
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
	bool InitMap(int lvl);
	bool InitGameOver();
	bool InitLevel1();
	bool InitLevel2();

private:
	//variables input
	unsigned char keys[256];
	bool jump_key;
	bool punch_key;
	bool up_key;
	bool down_key;

	//objectes principals del joc
	cRect cam;
	cScene Scene;
	cPlayer Player;
	cData Data;

	//variables de estat del joc
	int money;
	int stage;
	bool level_completed;
	int level;
	bool game_over;

	//variables extres
	int rectangle; //rectangle de la camara
	int rectangle_player; //rectangle del jugador
	
	int option;

	int arr_x,arr_y;
	int eat_x,eat_y;

	bool blank;
	bool reappears;
	int check_x, check_y;
	int delay;
	int eat_arrow;

	//variables de proves
	//std::vector<cMonster*> monsters;
	//std::vector<cBlock*> blocks;
	

	cRect GetBorder();
	int GetRectanglePlayer(const cPlayer &player);
	void CalculateCamResurrect();
};
