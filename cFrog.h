#pragma once
#include "cMonster.h"

class cFrog : public cMonster
{
public:
	cFrog(void);
	virtual ~cFrog(void);

	void SetState(int st);

	virtual void Draw(int tex_id);
	virtual void Logic(int *map, cPlayer &player, std::vector<cBlock*> &blocks, const cRect &rectangle, int level_width);
private:
	float y_ini;
	int down_delay;
	bool get_top;
};

