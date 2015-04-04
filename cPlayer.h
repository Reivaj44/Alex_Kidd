#pragma once

#include "cBicho.h"
#include <vector>
#include "cMonster.h"
#include "cBlock.h"

#define PLAYER_START_CX		3
#define PLAYER_START_CY		2

#define JUMP_HEIGHT		50
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
#define STATE_SWIMRIGHT		10
#define STATE_SWIMLEFT		11
#define STATE_SPUNCHLEFT	12
#define STATE_SPUNCHRIGHT	13
#define STATE_DEAD			14
#define STATE_RESURRECTED	15

class cMonster;

class cPlayer: public cBicho
{
public:
	cPlayer();
	virtual ~cPlayer();

	void SetWidthHeight(int w, int h);
	cRect GetHitBox();
	bool isPunching();
	bool isDead();

	void MoveRight(int *map, std::vector<cBlock*> &blocks);
	void MoveLeft(int *map, std::vector<cBlock*> &blocks);
	void Jump(int *map);
	void Crouch(int *map, std::vector<cBlock*> &blocks);
	void Punch(int *map);
	void Stop();
	void Die();
	void Poison();
	void Swim();
	void Resurrect(int tile_x, int tile_y);

	void SetState(int s);

	void Logic(int *map, std::vector<cMonster*> &monsters, std::vector<cBlock*> &blocks);

	virtual void Draw(int tex_id);

private:
	bool left;
	bool intheair;
	bool punching;
	bool swimming;

	bool jumping;
	int jump_alfa;
	int jump_y;
	bool down_press;

	cRect punchbox;
	cRect ipunchbox;
	int punch_delay;

	bool poisoned;
	int retard;

	virtual void UpdateBox();
	void ChangeBox();
};
