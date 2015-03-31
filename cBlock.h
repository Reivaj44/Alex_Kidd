#pragma once
#include "cbicho.h"

#define STATE_DESTROYED 0
#define STATE_BROWN 1
#define STATE_GREEN 2

class cBlock :
	public cBicho
{
public:
	cBlock(void);
	virtual ~cBlock(void);

	bool isDestroyed();
	virtual void Destroy();

	virtual void Draw(int tex_id);
};

