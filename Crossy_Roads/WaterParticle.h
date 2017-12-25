#pragma once
#include <glm\glm.hpp>

class WaterParticle {
	glm::vec3 pos;
	glm::vec3 speed;
public:
	WaterParticle();
	~WaterParticle();
};

