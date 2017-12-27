#include "AnimatedTexture.h"
using namespace glm;
using namespace std;

void AnimatedTexture::update(int deltaTime) {
	timeAnimation += deltaTime;
	while (timeAnimation > millisecsPerKeyframe) {
		timeAnimation -= millisecsPerKeyframe;
		currentKeyframe = (currentKeyframe + 1) % size;
	}
}

void AnimatedTexture::setTexures(const vector<IdTex>* textures, float millisecsPerKeyframe) {
	this->textures = textures;
	size = textures->size();
	this->millisecsPerKeyframe = millisecsPerKeyframe;
	currentKeyframe = 0;
	timeAnimation = 0;
}

IdTex AnimatedTexture::getCurrentTexure() const {
	return (*textures)[currentKeyframe];
}

AnimatedTexture::AnimatedTexture() : textures(NULL)
{
}

AnimatedTexture::~AnimatedTexture() {
}
