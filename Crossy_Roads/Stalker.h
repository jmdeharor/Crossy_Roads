#pragma once
#include "Behavior.h"

class Stalker : public Behavior {
public:
	const Object* objective;
	glm::vec2 direction;
	void update(int deltaTime) override;
	Stalker();
	~Stalker();
};

