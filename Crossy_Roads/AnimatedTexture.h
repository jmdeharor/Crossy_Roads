#pragma once
#include <vector>
#include "RandomPickMesh.h"

class AnimatedTexture {
	const std::vector<IdTex>* textures;
	glm::uint size;
	float timeAnimation;
	float millisecsPerKeyframe;
	glm::uint currentKeyframe;
public:
	void update(int deltaTime);
	void setTexures(const std::vector<IdTex>* textures, float millisecsPerKeyframe);
	IdTex getCurrentTexure() const;
	AnimatedTexture();
	~AnimatedTexture();
};

