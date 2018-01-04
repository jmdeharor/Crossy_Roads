#include "BehaviourObject.h"
#include "MonoBehaviour.h"


void BehaviourObject::update(int deltaTime) {
	if (behaviour != NULL)
		behaviour->update(deltaTime);
}

bool BehaviourObject::isAlive() const {
	return alive;
}

void BehaviourObject::destroy() {
	alive = false;
}

void BehaviourObject::setBehaviour(MonoBehaviour * behaviour) {
	behaviour->origin = this;
	this->behaviour = behaviour;
}

BehaviourObject::BehaviourObject() : behaviour(NULL), alive(true)
{
}


BehaviourObject::~BehaviourObject() {
	if (behaviour != NULL)
		delete behaviour;
}
