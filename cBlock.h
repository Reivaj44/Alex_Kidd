#pragma once
#include "cBicho.h"
#include "cPlayer.h"
#include "cMonster.h"

#define BMON  1
#define SMON  2
#define RING  3
#define LIFE  4
#define SKULL 5
#define QUEST 6
#define STAR  7
#define SKULL_P	8
#define R_BROWN 9
#define R_GREEN 10
#define CHBX  11
#define CHBXT 12
#define RICE  13


class cPlayer;
class cMonster;

class cBlock :
	public cBicho
{
public:
	cBlock(void);
	virtual ~cBlock(void);

	void SetTreasure(int treas);
	int GetTreasure();

	virtual bool Appears(const cRect& cam);
	virtual bool isCollisionable();
	virtual void Destroy();

	virtual void SetState(int st);

	virtual void Draw(int tex_id);
	virtual void Logic(cPlayer &player, int &money, std::vector<cMonster*> &monsters, int &check_x, int &check_y, bool &level_completed);

protected:
	bool collisionable;
	bool appears;
	static int rings_appeared;
	bool poisoned;
	bool touching;
	bool ghost_appears;
	int treasure;
	bool green;
	bool destroying;
	float xdes, ydes, xinc;

	void DrawRock(int tex_id);
	void Money(int &money, cPlayer &player);
};

