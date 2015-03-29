#pragma once

#include "cBicho.h"
#include <vector>
#include "cMonster.h"

#define PLAYER_START_CX		3
#define PLAYER_START_CY		2

#define JUMP_HEIGHT		96
#define JUMP_STEP		4

#define STATE_LOOKLEFT		0
#define STATE_LOOKRIGHT		1
#define STATE_WALKLEFT		2
#define STATE_WALKRIGHT		3
#define STATE_JUMPLEFT		4
#define STATE_JUMPRIGHT		5
#define STATE_CROUCHLEFT	6
#define STATE_CROUCHRIGHT	7
#define STATE_PUNCHLEFT		8
#define STATE_PUNCHRIGHT	9
#define STATE_DEAD			10

class cMonster;

class cPlayer: public cBicho
{
public:
	cPlayer();
	~cPlayer();

	void SetWidthHeight(int w, int h);
	cRect GetHitBox();
	bool isPunching();

	void MoveRight(int *map);
	void MoveLeft(int *map);
	void Jump(int *map);
	void Crouch(int *map);
	void Punch(int *map);
	void Stop();
	void Die();

	void SetState(int s);

	void Logic(int *map, std::vector<cMonster*> &monsters);

	virtual void Draw(int tex_id);

private:
	bool left;
	bool intheair;
	bool punching;

	bool jumping;
	int jump_alfa;
	int jump_y;

	cRect punchbox;
	cRect ipunchbox;

	void UpdateBox();
	void ChangeBox();
};
