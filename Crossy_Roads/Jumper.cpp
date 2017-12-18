#include "Jumper.h"
#include "Utils.h"
using namespace glm;

void Jumper::update() {
	switch (state) {
	case JumperState::Idle:
		modified = true;
		pos.x += horizontalSpeed;
		break;
	case JumperState::Jumping:
		modified = true;
		if (frameCount == jumpFrames) {
			pos.y = initialY;
			horizontalSpeed = -horizontalSpeed;
			frameCount = 0;
			state = JumperState::Idle;
		}
		else {
			pos.y += verticalSpeed;
			verticalSpeed += gravity;
			rot.y += angleSpeed;
			++frameCount;
		}
		break;
	}
}

void Jumper::jump() {
	if (state == JumperState::Idle) {
		state = JumperState::Jumping;
		verticalSpeed = initialSpeed;
		initialY = pos.y;
	}
}

Jumper::Jumper() : state(JumperState::Idle), frameCount(0) {
}

Jumper::~Jumper()
{
}

const float Jumper::gravity = -0.05f;
const uint Jumper::jumpFrames = 10;
const float Jumper::angleSpeed = PI / jumpFrames;
const float Jumper::initialSpeed = -0.5f*gravity*jumpFrames;