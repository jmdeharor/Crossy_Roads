#include "AnimTexObject.h"
using namespace glm;

void AnimTexObject::update(int deltatime) {
	timeAnimation += deltatime;
	while (timeAnimation > millisecsPerKeyframe) {
		timeAnimation -= millisecsPerKeyframe;
		currentKeyframe = (currentKeyframe + 1) % size;
	}
}

void AnimTexObject::setTextures(std::pair<IdMesh, glm::uint> textures, float millisecsPerKeyframe) {
	firstTexture = textures.first;
	size = textures.second;
	this->millisecsPerKeyframe = millisecsPerKeyframe;
	currentKeyframe = 0;
	timeAnimation = 0;
}

IdTex AnimTexObject::getTexture() const {
	return firstTexture + currentKeyframe;
}

AnimTexObject::AnimTexObject()
{
}


AnimTexObject::~AnimTexObject()
{
}
