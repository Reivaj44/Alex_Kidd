#pragma once

#include "cBlock.h"
#include "cPlayer.h"

#define STATE_BMON  1
#define STATE_SMON  2
#define STATE_RING  3
#define STATE_SKULL 4
#define STATE_QUEST 5
#define STATE_STAR  6

class cBox :
	public cBlock
{
public:
	cBox(void);
	virtual ~cBox(void);

	void SetTreasure(int treas);
	int GetTreasure();

	virtual bool Appears();
	virtual bool isCollisionable();
	virtual void Destroy();

	virtual void SetState(int st);

	virtual void Draw(int tex_id);
	virtual void Logic(cPlayer player);

protected:
	bool collisionable;
	bool appears;
	int treasure;
};

