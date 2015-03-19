#pragma once

#include "cBicho.h"

#define PLAYER_START_CX		3
#define PLAYER_START_CY		2

class cPlayer: public cBicho
{
public:
	cPlayer();
	~cPlayer();

	void MoveRight(int *map);
	void MoveLeft(int *map);
	void Jump(int *map);
	void Crouch(int *map);
	void Punch(int *map);
	void Stop();
	void Logic(int *map);

	void Draw(int tex_id);

private:
	bool lright;
	bool intheair;
};
