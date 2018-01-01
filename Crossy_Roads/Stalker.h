#pragma once
#include "MonoBehaviour.h"

class Stalker : public MonoBehaviour {
public:
	const Object* objective;
	glm::vec2 direction;
	void update(int deltaTime) override;
	Stalker();
	~Stalker();
};

