#include "Player.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Game.h"
#include "Pi.h"
using namespace glm;

#define FRAMES_PER_SECOND 60
#define JUMP_DURATION 10

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
	vec3 bbox = playerMesh.getbbCenter();
	//playerObject.setCenter(vec3(bbox.x, bbox.y - playerMesh.getHeight()/2.f, bbox.z));
	playerObject.setPos(vec3(0, playerMesh.getHeight()*0.1f/2, 0));
	playerObject.setPlane(vec4(0, 1, 0, 0), vec3(1, 1, 0));
	frontPressed = backPressed = leftPressed = rightPressed = false;
	wPressed = aPressed = sPressed = dPressed = false;
	currentOrientation = FRONT;
	directionVector = vec3(0, 0, 1.f);
	inMovement = false;
	gravity = -0.1;
	verticalSpeed = getJumpingSpeed(JUMP_DURATION);
	speed = this->jumpDistance / float(JUMP_DURATION-1);
	testJump = 0;
	currentFrame = 0;
}

bool Player::update(int deltaTime) {
	if (inMovement) {
		if (!keepMoving()) {
			currentFrame = 0;
			inMovement = false;
		}
	}

	if (!inMovement) {
		if (Game::instance().getKey('w')) {
			if (!wPressed) {
				wPressed = true;
				performRotation(currentOrientation, 'w');
				currentOrientation = FRONT;
				setDirectionVector();
				inMovement = true;
			}
		}
		else
			wPressed = false;
		if (Game::instance().getKey('a')) {
			if (!aPressed) {
				aPressed = true;
				performRotation(currentOrientation, 'a');
				currentOrientation = LEFT;
				setDirectionVector();
				inMovement = true;
			}
		}
		else
			aPressed = false;
		if (Game::instance().getKey('d')) {
			if (!dPressed) {
				dPressed = true;
				performRotation(currentOrientation, 'd');
				currentOrientation = RIGHT;
				setDirectionVector();
				inMovement = true;
			}
		}
		else
			dPressed = false;
		if (Game::instance().getKey('s')) {
			if (!sPressed) {
				sPressed = true;
				performRotation(currentOrientation, 's');
				currentOrientation = BACK;
				setDirectionVector();
				inMovement = true;
			}
		}
		else
			sPressed = false;
	}
	
	if (Game::instance().getKey('q')) {
		if (!leftPressed) {
			leftPressed = true;
			playerObject.rotateY(PI/2.f);
			currentOrientation = Orientation(currentOrientation + 1);
			if (currentOrientation == 4) currentOrientation = FRONT;
		}
	}
	else
		leftPressed = false;
	if (Game::instance().getKey('e')) {
		if (!rightPressed) {
			rightPressed = true;
			playerObject.rotateY(-PI/2.f);
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
		directionVector = vec3(0, 0, 1);
		break;
	case LEFT:
		directionVector = vec3(1, 0, 0);
		break;
	case BACK:
		directionVector = vec3(0, 0, -1);
		break;
	case RIGHT:
		directionVector = vec3(-1, 0, 0);
		break;
	}
}

void Player::performRotation(Orientation currentOrientation, char key) {
	switch (key) {
	case 'w':
		switch (currentOrientation) {
		case BACK:
			playerObject.rotateY(PI);
			break;
		case LEFT:
			playerObject.rotateY(-PI / 2.f);
			break;
		case RIGHT:
			playerObject.rotateY(PI / 2.f);
			break;
		case FRONT:
		default:
			break;
		}
		break;
	case 'a':
		switch (currentOrientation) {
		case FRONT:
			playerObject.rotateY(PI / 2.f);
			break;
		case BACK:
			playerObject.rotateY(-PI / 2.f);
			break;
		case RIGHT:
			playerObject.rotateY(PI);
			break;
		case LEFT:
		default:
			break;
		}
		break;
	case 'd':
		switch (currentOrientation) {
		case FRONT:
			playerObject.rotateY(-PI / 2.f);
			break;
		case BACK:
			playerObject.rotateY(PI / 2.f);
			break;
		case LEFT:
			playerObject.rotateY(PI);
			break;
		case RIGHT:
		default:
			break;
		}
		break;
	case 's':
		switch (currentOrientation) {
		case FRONT:
			playerObject.rotateY(PI);
			break;
		case LEFT:
			playerObject.rotateY(PI / 2.f);
			break;
		case RIGHT:
			playerObject.rotateY(-PI / 2.f);
			break;
		case BACK:
		default:
			break;
		}
		break;
	default:
		break;
	}
}

bool Player::keepMoving() {
	currentFrame++;
	bool returnValue = true;
	currentVerticalSpeed = verticalSpeed + gravity*currentFrame;
	if (playerObject.getPos().y + currentVerticalSpeed <= 0) {
		currentVerticalSpeed = -playerObject.getPos().y;
		returnValue = false;
	}
	vec3 horizontalMove = directionVector*speed;
	vec3 verticalMove = vec3(0, currentVerticalSpeed, 0);
	playerObject.move(horizontalMove + verticalMove);
	return returnValue;
}

float Player::getJumpingSpeed(uint frames) {
	return -0.5f*gravity*frames;
}

Player::Player(){
}

Player::~Player()
{
}
