#pragma once
#include "Object.h"

class MonoBehavior {
public:
	Object* origin;
	virtual void update(int deltaTime) = 0;
	MonoBehavior();
	virtual ~MonoBehavior();
};

