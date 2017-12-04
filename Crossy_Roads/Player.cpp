#include "Player.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Game.h"
#include "Pi.h"
using namespace glm;

#define FRAMES_PER_SECOND 60
#define JUMP_DURATION 11

void Player::firstInit() {
}

void Player::groupDrawableObjects(std::vector<std::vector<Object*>>& objects, std::vector<std::vector<TexturedObject*>>& texturedObjects, const FrustumG& frustum) {
	objects[playerObject.meshId].push_back(&playerObject);
}

void Player::jump() {
}

void Player::init(const Assets& assets, vec3 lightDir, vec3 offset, float jumpDistance, const Floor &floor) {
	GameObject::init();
	this->lightDir = lightDir;
	this->floor = &floor;
	this->jumpDistance = jumpDistance;
	IdMesh pirateId = assets.getMeshId("pirate_2");
	playerObject.setMesh(pirateId, assets.getMesh(pirateId));
	playerObject.setScale(vec3(0.1f));
	playerObject.setCenterToBaseCenter();
	playerObject.setPos(vec3(0));
	playerObject.setPlane(vec4(0, 1, 0, 0), lightDir);
	wPressed = aPressed = sPressed = dPressed = false;
	currentOrientation = FRONT;
	directionVector = vec3(0, 0, 1.f);
	inMovement = false;
	gravity = -0.1f;
	verticalSpeed = getJumpingSpeed(0,0,JUMP_DURATION);
	speed = this->jumpDistance / float(JUMP_DURATION);
	testJump = 0;
	currentFrame = 0;
	currentRowIndex = floor.getRows()/2;
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
				uint previousRowIndex = currentRowIndex;
				currentRowIndex = (currentRowIndex + 1) % floor->getRows();
				float prevHeight = floor->getFloorRow(previousRowIndex)->getHeight();
				float currentHeight = floor->getFloorRow(currentRowIndex)->getHeight();
				verticalSpeed = getJumpingSpeed(prevHeight, currentHeight, JUMP_DURATION);

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
				uint previousRowIndex = currentRowIndex;
				if (currentRowIndex == 0) currentRowIndex = floor->getRows() - 1;
				else currentRowIndex = (currentRowIndex - 1) % floor->getRows();
				float prevHeight = floor->getFloorRow(previousRowIndex)->getHeight();
				float currentHeight = floor->getFloorRow(currentRowIndex)->getHeight();
				verticalSpeed = getJumpingSpeed(prevHeight, currentHeight, JUMP_DURATION);

			}
		}
		else
			sPressed = false;
	}
	return ret;
}

void Player::render(ShaderProgram & program) {
	Scene::sceneTriangles += playerObject.getTriangles();
	Scene::sceneDrawCalls += 1;
	playerObject.render(program);
}

void Player::renderShadow(ShaderProgram & program) {
	Scene::sceneDrawCalls += 1;
	Scene::sceneTriangles += playerObject.getTriangles();
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
	float auxHeight = floor->getFloorRow(currentRowIndex)->getHeight();
	currentFrame++;
	bool returnValue = true;
	playerObject.setPlane(vec4(0, 1, 0, -auxHeight), lightDir);

	currentVerticalSpeed = verticalSpeed + gravity*currentFrame;
	if (currentFrame == JUMP_DURATION) {
		currentVerticalSpeed =  -(playerObject.getPos().y - auxHeight);
		returnValue = false;
	}
	vec3 horizontalMove = directionVector*speed;
	vec3 verticalMove = vec3(0, currentVerticalSpeed, 0);
	playerObject.move(horizontalMove + verticalMove);
	return returnValue;
}

float Player::getJumpingSpeed(float y0, float y, uint frames) {
	return (y-y0)/frames -0.5f*gravity*frames;
}

Player::Player(){
}

Player::~Player()
{
}
