#pragma once
#include "cptero.h"

class cSFish :	public cPtero
{
public:
	cSFish(void);
	virtual ~cSFish(void);

	virtual void Draw(int tex_id);
};

