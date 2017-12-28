#include "AnimMeshObject.h"
using namespace glm;

void AnimMeshObject::update(int deltatime) {
	timeAnimation += deltatime;
	while (timeAnimation > millisecsPerKeyframe) {
		timeAnimation -= millisecsPerKeyframe;
		currentKeyframe = (currentKeyframe + 1) % size;
	}
}

void AnimMeshObject::setMeshes(uint size, float millisecsPerKeyframe) {
	this->size = size;
	this->millisecsPerKeyframe = millisecsPerKeyframe;
	currentKeyframe = 0;
	timeAnimation = 0;
}

IdMesh AnimMeshObject::getMesh() const {
	return meshId + currentKeyframe;
}

AnimMeshObject::AnimMeshObject()
{
}


AnimMeshObject::~AnimMeshObject()
{
}
