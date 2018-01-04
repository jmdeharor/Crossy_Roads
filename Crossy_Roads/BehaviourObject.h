#pragma once
#include "Object.h"

class MonoBehaviour;

class BehaviourObject : public Object {
	MonoBehaviour* behaviour;
	bool alive;
public:
	void update(int deltaTime);
	bool isAlive() const;
	void destroy();
	void setBehaviour(MonoBehaviour* behaviour);
	BehaviourObject();
	~BehaviourObject();
};

