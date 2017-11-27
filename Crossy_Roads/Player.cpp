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

void Player::init(vec3 lightDir, vec3 offset, float jumpDistance, const Floor &floor) {
	GameObject::init();
	this->floor = &floor;
	this->jumpDistance = jumpDistance;
	playerObject.setMesh(&playerMesh);
	playerObject.setScale(vec3(0.1f));
	playerObject.setCenterToBaseCenter();
	playerObject.setPos(vec3(0));
	playerObject.setPlane(vec4(0, 1, 0, 0), lightDir);
	wPressed = aPressed = sPressed = dPressed = false;
	currentOrientation = FRONT;
	directionVector = vec3(0, 0, 1.f);
	inMovement = false;
	gravity = -0.1;
	verticalSpeed = getJumpingSpeed(JUMP_DURATION);
	speed = this->jumpDistance / float(JUMP_DURATION-1);
	testJump = 0;
	currentFrame = 0;
	currentRowIndex = 24;
}

PlayerReturn Player::update(int deltaTime) {
	PlayerReturn ret = PlayerReturn::NOTHING;
	if (inMovement) {
		if (!keepMoving()) {
			currentFrame = 0;
			inMovement = false;
		}
	}
	else {
		if (Game::instance().getKey('w')) {
			if (!wPressed) {
				ret = PlayerReturn::MOVE_FRONT;
				wPressed = true;
				performRotation(currentOrientation, 'w');
				currentOrientation = FRONT;
				setDirectionVector();
				inMovement = true;
				currentRowIndex = (currentRowIndex + 1) % floor->getRows();
			}
		}
		else
			wPressed = false;
		if (Game::instance().getKey('a')) {
			if (!aPressed) {
				ret = PlayerReturn::MOVE_LEFT;
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
				ret = PlayerReturn::MOVE_RIGHT;
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
				ret = PlayerReturn::MOVE_BACK;
				sPressed = true;
				performRotation(currentOrientation, 's');
				currentOrientation = BACK;
				setDirectionVector();
				inMovement = true;
				if (currentRowIndex == 0) currentRowIndex = floor->getRows() - 1;
				else currentRowIndex = (currentRowIndex - 1) % floor->getRows();
			}
		}
		else
			sPressed = false;
	}
	return ret;
}

void Player::render(ShaderProgram & program) {
	playerObject.render(program);
}

void Player::renderShadow(ShaderProgram & program) {
	playerObject.renderShadow(program);
}

vec3 Player::getPos() const {
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
		//playerObject.setRotationY(0);
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
		//playerObject.setRotationY(PI/2);
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
		//playerObject.setRotationY(-PI / 2);
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
		//playerObject.setRotationY(PI);
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
