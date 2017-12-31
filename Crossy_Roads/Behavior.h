#pragma once
#include "Object.h"

class Behavior {
public:
	Object* origin;
	virtual void update(int deltaTime) = 0;
	Behavior();
	virtual ~Behavior();
};

