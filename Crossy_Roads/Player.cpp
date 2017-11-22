#include "Player.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Game.h"
using namespace glm;

#define PI 3.14159265359

void Player::firstInit() {
	playerMesh.loadFromFile("models/pirate.obj");
}

void Player::jump() {
}

void Player::init(vec3 lightDir, vec3 offset, float jumpDistance) {
	GameObject::init();
	this->jumpDistance = jumpDistance;
	playerObject.setMesh(&playerMesh);
	playerObject.setScale(vec3(0.1f));
	playerObject.setPos(vec3(0, playerObject.getHeight() / 2, 0) + offset);
	playerObject.setPlane(vec4(0, 1, 0, 0), vec3(1, 1, 0));
	frontPressed = backPressed = leftPressed = rightPressed = false;
	currentOrientation = FRONT;
	directionVector = vec3(0, 0, jumpDistance);
}

bool Player::update(int deltaTime) {
	if (Game::instance().getKey('w')) {
		if (!frontPressed) {
			frontPressed = true;
			setDirectionVector();
			playerObject.move(directionVector);
		}
	}
	else
		frontPressed = false;
	if (Game::instance().getKey('q')) {
		if (!leftPressed) {
			leftPressed = true;
			playerObject.rotateY(PI/2.);
			currentOrientation = Orientation(currentOrientation + 1);
			if (currentOrientation == 4) currentOrientation = FRONT;
		}
	}
	else
		leftPressed = false;
	if (Game::instance().getKey('e')) {
		if (!rightPressed) {
			rightPressed = true;
			playerObject.rotateY(-PI/2.);
			currentOrientation = Orientation(currentOrientation - 1);
			if (currentOrientation == -1) currentOrientation = RIGHT;
		}
	}
	else
		rightPressed = false;
	return true;
}

void Player::render(ShaderProgram & program) {
	playerObject.render(program);
}

void Player::renderShadow(ShaderProgram & program) {
	playerObject.renderShadow(program);
}

vec3 Player::getPos() {
	return playerObject.getPos();
}

void Player::setDirectionVector() {
	switch (currentOrientation) {
	case FRONT:
		directionVector = vec3(0, 0, jumpDistance);
		break;
	case LEFT:
		directionVector = vec3(jumpDistance, 0, 0);
		break;
	case BACK:
		directionVector = vec3(0, 0, -jumpDistance);
		break;
	case RIGHT:
		directionVector = vec3(-jumpDistance, 0, 0);
		break;
	}
}

Player::Player(){
}

Player::~Player()
{
}
