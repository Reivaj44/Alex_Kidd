#pragma once

#include "cbicho.h"

class cPtero :	public cBicho
{
public:
	cPtero(void);
	~cPtero(void);

	void Draw(int tex_id);
	void Move(int *map);
};

