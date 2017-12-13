#pragma once
#include "ShadowedObject.h"

enum class JumperState {
	Idle,
	Jumping
};

class Jumper : public ShadowedObject {
	JumperState state;
	float verticalSpeed;
	float initialY;
	glm::uint frameCount;
	const static float angleSpeed;
	const static float gravity;
	const static float initialSpeed;
	const static glm::uint jumpFrames;
public:
	float horizontalSpeed;
public:
	void update();
	void jump();
	Jumper();
	~Jumper();
};

