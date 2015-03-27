#pragma once

#include "cMonster.h"

class cPtero :	public cMonster
{
public:
	cPtero(void);
	~cPtero(void);

	void Die();
	bool isDead();
	virtual void Draw(int tex_id);
	virtual void Logic(int *map);
};

