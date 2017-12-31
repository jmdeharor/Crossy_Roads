#include "Stalker.h"
#include "Utils.h"
using namespace glm;


void Stalker::update(int deltaTime) {
	if (origin->mesh == NULL)
		int a = 3;
	vec2 vDist = normalize(objective->getHorizontalPlanePos() - origin->getHorizontalPlanePos());
	float dot = vDist.x*direction.x + vDist.y*direction.y;
	float det = vDist.x*direction.y - vDist.y*direction.x;
	float angle = atan2(det, dot);
	origin->setRotationY(angle);
}

Stalker::Stalker() {
	origin = NULL;
}


Stalker::~Stalker()
{
}
