#pragma once
#include "Object.h"

class AnimTexObject : public Object {
	IdTex firstTexture;
	glm::uint size;
	float timeAnimation;
	float millisecsPerKeyframe;
	glm::uint currentKeyframe;
public:
	void update(int deltatime);
	void setTextures(std::pair<IdTex, glm::uint> textures, float millisecsPerKeyframe);
	IdTex getTexture() const;
	AnimTexObject();
	~AnimTexObject();
};

