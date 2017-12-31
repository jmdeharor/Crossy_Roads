#include "Stalker.h"
using namespace glm;


void Stalker::update(int deltaTime) {
	vec2 vDist = normalize(objective->getHorizontalPlanePos() - origin->getHorizontalPlanePos());
	float angle = acos(dot(vDist, direction));
	origin->setRotationY(angle);
}

Stalker::Stalker()
{
}


Stalker::~Stalker()
{
}
