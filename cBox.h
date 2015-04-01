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

	virtual void Destroy();

	virtual void Draw(int tex_id);
	void Logic(cPlayer player);

protected:
	bool box;
	int treasure;
};

