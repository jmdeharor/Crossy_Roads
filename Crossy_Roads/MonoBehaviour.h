#pragma once
#include "BehaviourObject.h"

class MonoBehaviour {
public:
	BehaviourObject* origin;
	virtual void start() {};
	virtual void update(int deltaTime) = 0;
	MonoBehaviour();
	virtual ~MonoBehaviour();
};

