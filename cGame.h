#pragma once

#include "cScene.h"
#include "cPlayer.h"
#include "cMonster.h"
#include "cData.h"
#include "cPtero.h"

#define GAME_WIDTH	640
#define GAME_HEIGHT 480

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
	double cam_x, cam_y;
	cScene Scene;
	cPlayer Player;
	std::vector<cMonster*> monsters;
	cData Data;
};