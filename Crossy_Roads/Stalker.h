#pragma once
#include "MonoBehavior.h"

class Stalker : public MonoBehavior {
public:
	const Object* objective;
	glm::vec2 direction;
	void update(int deltaTime) override;
	Stalker();
	~Stalker();
};

