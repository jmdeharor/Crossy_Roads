#pragma once
#include "Object.h"

class MonoBehaviour {
public:
	Object* origin;
	virtual void update(int deltaTime) = 0;
	MonoBehaviour();
	virtual ~MonoBehaviour();
};

