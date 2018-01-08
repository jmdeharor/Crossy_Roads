#include "Player.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Game.h"
#include "Utils.h"
using namespace glm;

#define FRAMES_PER_SECOND 60
#define JUMP_DURATION 11

void Player::firstInit() {
	soundManager = Game::instance().getSoundManager();
	jumpSound = soundManager->loadSound("sounds/Effect_jump.wav", FMOD_DEFAULT);
	waterSplashSound = soundManager->loadSound("sounds/Effect_water_splash.wav", FMOD_CREATESTREAM);
	deathCrush = soundManager->loadSound("sounds/Effect_death_crush.wav", FMOD_DEFAULT);
	deathOut = soundManager->loadSound("sounds/Effect_death_out.wav", FMOD_DEFAULT);
	game = &Game::instance();
}

void Player::groupDrawableObjects(const FrustumG& frustum, RenderVectors& renderVectors) {
	if (playerObject.isInsideViewFrustum(frustum)) {
		outOfTheScene = false;
		if (state == PlayerState::DeadByEnemy) {
			renderVectors.texturedObjects[textureObject.texture].push_back(&textureObject);
		}
		else if (state == PlayerState::DeadByLava) {
			renderVectors.projectionShadowObjects[playerObject.meshId].push_back(&playerObject);
		}
		else {
			renderVectors.objects[playerObject.meshId].push_back(&playerObject);
			renderVectors.shadowMapOpjects[playerObject.meshId].push_back(&playerObject);
		}
	}
	else {
		renderVectors.shadowMapOpjects[playerObject.meshId].push_back(&playerObject);
		outOfTheScene = true;
	}
}

void Player::init(const Assets& assets, vec3 lightDir, vec3 offset, float jumpDistance, Floor& floor, WaterParticleSystem* particleSystem) {
	leftRight = 0;
	GameObject::init();
	this->lightDir = lightDir;
	this->floor = &floor;
	this->particleSystem = particleSystem;
	this->jumpDistance = jumpDistance;
	this->assets = &assets;
	playerModels = assets.getFreeGroup(FreeMeshGroup::Player);

	currentPosScore = score = 0;

	currentRowIndex = floor.getRows() / 2 - floor.getRowOffset();
	currentColIndex = floor.getCols() / 2 - floor.getColOffset();
	vec3 rowHeight = floor.getFloorRow(currentRowIndex)->getNextPos(currentColIndex).first;

	charSelected = Game::instance().charSelected;

	IdMesh playerModelId = (*playerModels)[charSelected];
	playerObject.setMesh(playerModelId, assets.getMesh(playerModelId));
	playerObject.setScale(vec3(assets.getScale(playerModelId)));
	playerObject.setCenterToBaseCenter();
	playerObject.setPos(vec3(0,rowHeight.y,0));
	playerObject.setPlane(vec4(0, 1, 0, -rowHeight.y), lightDir);

	wPressed = aPressed = sPressed = dPressed = false;
	currentOrientation = FRONT;
	directionVector = vec3(0, 0, 1.f);
	inMovement = false;
	gravity = -0.1f;
	verticalSpeed = getJumpingSpeed(0,0,JUMP_DURATION);
	speed = this->jumpDistance / float(JUMP_DURATION);
	testJump = 0;
	currentFrame = 0;
	outOfTheScene = false;
	state = PlayerState::Alive;

	const Mesh* cubeMesh = assets.getCubeMesh();
	vec3 bbSize = cubeMesh->getbbSize();
	textureObject.setMesh(cubeMesh);
	textureObject.setScale(vec3(3, 0.1f, 2)/bbSize);
	textureObject.setRotationY(PI / 2);

	nextPos.y = playerObject.getY();
	godModePressed = false;
	godMode = false;
	playerControl = true;
	mousePressed = false;
}

PlayerReturn Player::update(int deltaTime) {
	if (game->charSelected != charSelected) {
		charSelected = Game::instance().charSelected;
		IdMesh playerModelId = (*playerModels)[charSelected];
		playerObject.setMesh(playerModelId, assets->getMesh(playerModelId));
		playerObject.setScale(vec3(assets->getScale(playerModelId)));
		playerObject.setCenterToBaseCenter();
	}

	if (Game::instance().getKey('g')) {
		godModePressed = true;
	}
	else if (godModePressed) {
		godMode = !godMode;
		godModePressed = false;
	}

	if (state == PlayerState::DeadByLava) {
		vec3 scale = playerObject.getScale();
		if (scale.x > 0) {
			scale -= 0.0001f;
			if (scale.x < 0)
				scale = vec3(0);
			playerObject.setScale(scale);
		}
	}

	if (outOfTheScene && !godMode && state == PlayerState::Alive) {
		platformSpeed = 0;
		FMOD::Channel* channel = soundManager->playSound(deathOut);
		channel->setVolume(0.3f);
		state = PlayerState::DeadByOut;
		return PlayerReturn::DEAD;
	}
	currentColIndex = FloorRow::worldToCol(playerObject.getPos().x);
	PlayerReturn ret = PlayerReturn::NOTHING;
	if (collides() && !godMode && playerControl) {
		particleSystem->trigger(playerObject.getPos(), 30, vec4(1, 0, 0, 0));
		textureObject.setPos(vec3(nextPos.x, nextPos.y+0.001f, nextPos.z));
		textureObject.texture = assets->getTextureId("char_unlocked_" + to_string(charSelected));
		FMOD::Channel *channel = soundManager->playSound(deathCrush);
		state = PlayerState::DeadByEnemy;
		return PlayerReturn::DEAD;
	}
	if (inMovement) {
		if (!keepMoving()) {
			currentFrame = 0;
			inMovement = false;
			currentColIndex = FloorRow::worldToCol(playerObject.getPos().x);
			FloorRow* floorRow = floor->getFloorRow(currentRowIndex);
			float rowHeight = floorRow->getRowHeight();
			if (rowHeight == playerObject.getY() && floorRow->isTheFloorLava() && !godMode) {
				particleSystem->trigger(playerObject.getPos(), 17, vec4(0,0.4f,0.86f,0));
				soundManager->playSound(waterSplashSound);
				playerObject.setPlane(vec4(0, 1, 0, -rowHeight - 0.001f), vec3(0,1,0));
				state = PlayerState::DeadByLava;
				return PlayerReturn::DEAD;
			}
		}
	}
	else if (playerControl) {
		playerObject.moveX(platformSpeed);

		if (game->getMouseControl()) {
			if (game->getLeftButtonPressed()) {
				mousePressed = true;
			}
			else if (mousePressed) {
				mousePressed = false;
				int x = game->getX();
				int y = game->getY();
				/*if (y < SCREEN_HEIGHT / 2.f) {
					move(FRONT);
					ret = PlayerReturn::MOVE_FRONT;
				}
				else if (x < SCREEN_WIDTH*(1.f/3.f)) {
					move(LEFT);
					ret = PlayerReturn::MOVE_LEFT;
				}
				else if (x >= SCREEN_WIDTH*(1.f / 3.f) && x < SCREEN_WIDTH*(2.f / 3.f)) {
					move(BACK);
					ret = PlayerReturn::MOVE_BACK;
				}
				else {
					move(RIGHT);
					ret = PlayerReturn::MOVE_RIGHT;
				}*/
				static const uint segmentSize = 50;
				if (y <= 535) {
					float left = SCREEN_WIDTH / 2.f - segmentSize / 2.f;
					float x1 = 0;
					float y1 = 0;
					float x2 = left;
					float y2 = 535;
					float aux;
					if (leftRight > 3) {
						aux = (leftRight - 3) * 50;
						x2 += aux;
					}
					if (leftRight < -3) {
						aux = (leftRight + 3) * 50;
						x2 += aux;
					}

					float slope1 = (y2 - y1) / (x2 - x1);
					float slope2 = -slope1;
					float offset1 = -x1 * (y2 - y1) / (x2 - x1) + y1;
					x1 = SCREEN_WIDTH;
					y1 = 0;
					x2 = SCREEN_WIDTH / 2.f + segmentSize / 2.f;
					y2 = 535;
					float offset2 = -x1 * (y2 - y1) / (x2 - x1) + y1;
					float yfinal1 = slope1*x + offset1;
					float yfinal2 = slope2 * x + offset2;
					if (yfinal1 < y) {
						move(LEFT);
						leftRight -= 1;
						ret = PlayerReturn::MOVE_LEFT;
					}
					else if (yfinal2 < y) {
						move(RIGHT);
						leftRight += 1;
						ret = PlayerReturn::MOVE_RIGHT;
					}
					else {
						move(FRONT);
						ret = PlayerReturn::MOVE_FRONT;
					}
				}
				else {
					float left = SCREEN_WIDTH / 2.f - segmentSize / 2.f;
					float x1 = 0;
					float y1 = SCREEN_HEIGHT;
					float x2 = left;
					float y2 = 535;
					float aux;
					if (leftRight > 3) {
						aux = (leftRight - 3) * 50;
						x2 += aux;
					}
					if (leftRight < -3) {
						aux = (leftRight + 3) * 50;
						x2 += aux;
					}
					float slope1 = (y2 - y1) / (x2 - x1);
					float slope2 = -slope1;
					float offset1 = -x1 * (y2 - y1) / (x2 - x1) + y1;
					x1 = SCREEN_WIDTH;
					y1 = SCREEN_HEIGHT;
					x2 = SCREEN_WIDTH / 2.f + segmentSize / 2.f;
					y2 = 535;
					float offset2 = -x1 * (y2 - y1) / (x2 - x1) + y1;
					float yfinal1 = slope1 * x + offset1;
					float yfinal2 = slope2 * x + offset2;
					if (yfinal1 >= y) {
						move(LEFT);
						leftRight -= 1;
						ret = PlayerReturn::MOVE_LEFT;
					}
					else if (yfinal2 >= y) {
						move(RIGHT);
						leftRight += 1;
						ret = PlayerReturn::MOVE_RIGHT;
					}
					else {
						move(BACK);
						ret = PlayerReturn::MOVE_BACK;
					}
				}
			}
		}
		else {
			if (game->getKey('w')) {
				if (!wPressed) {
					ret = PlayerReturn::MOVE_FRONT;
					wPressed = true;
					move(FRONT);
				}
				aPressed = dPressed = sPressed = false;
			}
			else if (game->getKey('a')) {
				if (!aPressed) {
					ret = PlayerReturn::MOVE_LEFT;
					aPressed = true;
					move(LEFT);
				}
				wPressed = dPressed = sPressed = false;
			}
			else if (game->getKey('d')) {
				if (!dPressed) {
					ret = PlayerReturn::MOVE_RIGHT;
					dPressed = true;
					move(RIGHT);
				}
				wPressed = aPressed = sPressed = false;
			}
			else if (game->getKey('s')) {
				if (!sPressed) {
					ret = PlayerReturn::MOVE_BACK;
					sPressed = true;
					move(BACK);
				}
				wPressed = dPressed = aPressed = false;
			}
			else
				wPressed = aPressed = dPressed = sPressed = false;
		}
	}
	return ret;
}

vec3 Player::getPos() const {
	return playerObject.getPos();
}

float Player::getHeight() const {
	return nextPos.y;
}

const Object * Player::getObject() const
{
	return &playerObject;
}

void Player::calculateSpeeds() {
	vec3 playerPos = playerObject.getPos();
	verticalSpeed = getJumpingSpeed(playerPos.y, nextPos.y, JUMP_DURATION);
	speeds.x = (nextPos.x - playerPos.x) / JUMP_DURATION;
	speeds.z = (nextPos.z - playerPos.z) / JUMP_DURATION;
}

void Player::disablePlayerControl() {
	playerControl = false;
}

void Player::enablePlayerControl() {
	playerControl = true;
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

void Player::performRotation(char key) {
	switch (key) {
	case 'w':
		playerObject.setRotationY(0);
		break;
	case 'a':
		playerObject.setRotationY(PI/2);
		break;
	case 'd':
		playerObject.setRotationY(-PI / 2);
		break;
	case 's':
		playerObject.setRotationY(PI);
		break;
	}
}

bool Player::keepMoving() {
	currentFrame++;
	bool returnValue;

	if (currentFrame == JUMP_DURATION) {
		playerObject.setPos(nextPos);
		returnValue = false;
	}
	else {
		returnValue = true;
		speeds.y = verticalSpeed + gravity*currentFrame;
		playerObject.move(speeds);
	}
	return returnValue;
}

float Player::getJumpingSpeed(float y0, float y, uint frames) {
	return (y-y0)/frames -0.5f*gravity*frames;
}

bool Player::collides() {
	bool collision = false;
	FloorRow* currentRow = floor->getFloorRow(currentRowIndex);
	vector<Jumper>* rowEnemies = currentRow->getEnemies();
	for (uint i = 0; i < rowEnemies->size() && !collision; ++i) {
		collision = playerObject.collidesWith((*rowEnemies)[i]);
	}
	return collision;
}

bool Player::collidesWithEnv(uint row, uint col) {
	FloorRow* rowToCheck = floor->getFloorRow(row);
	if (!rowToCheck->isSafeZone() || rowToCheck->getBiome() == Sea)
		return false;
	vector<CellProperties>* rowObjects = rowToCheck->getRowObjects();
	return (*rowObjects)[col].collision;
}

void Player::move(Orientation orientation) {
	uint nextRow;
	switch (orientation) {
	case FRONT:
		nextRow = (currentRowIndex + 1) % floor->getRows();
		if (!collidesWithEnv(nextRow, currentColIndex)) {
			performRotation('w');
			currentOrientation = FRONT;
			setDirectionVector();
			inMovement = true;
			uint previousRowIndex = currentRowIndex;
			currentRowIndex = nextRow;
			pair<vec3, float> currentPos = floor->getFloorRow(currentRowIndex)->getNextPos(currentColIndex);
			nextPos = currentPos.first;
			platformSpeed = currentPos.second;
			calculateSpeeds();
			soundManager->playSound(jumpSound);
			currentPosScore += 1;
			score = max(currentPosScore, score);
		}
		break;
	case LEFT:
		if (!collidesWithEnv(currentRowIndex, currentColIndex + 1)) {
			performRotation('a');
			currentOrientation = LEFT;
			setDirectionVector();
			inMovement = true;
			currentColIndex += 1;
			pair<vec3, float> currentPos = floor->getFloorRow(currentRowIndex)->getNextPos(currentColIndex);
			nextPos = currentPos.first;
			platformSpeed = currentPos.second;
			calculateSpeeds();
			soundManager->playSound(jumpSound);
		}
		break;
	case RIGHT:
		if (!collidesWithEnv(currentRowIndex, currentColIndex - 1)) {
			performRotation('d');
			currentOrientation = RIGHT;
			setDirectionVector();
			inMovement = true;
			currentColIndex -= 1;
			pair<vec3, float> currentPos = floor->getFloorRow(currentRowIndex)->getNextPos(currentColIndex);
			nextPos = currentPos.first;
			platformSpeed = currentPos.second;
			calculateSpeeds();
			soundManager->playSound(jumpSound);
		}
		break;
	case BACK:
		nextRow = currentRowIndex == 0 ? floor->getRows() - 1 : currentRowIndex - 1;
		if (!collidesWithEnv(nextRow, currentColIndex)) {
			performRotation('s');
			currentOrientation = BACK;
			setDirectionVector();
			inMovement = true;
			uint previousRowIndex = currentRowIndex;
			currentRowIndex = nextRow;
			pair<vec3, float> currentPos = floor->getFloorRow(currentRowIndex)->getNextPos(currentColIndex);
			nextPos = currentPos.first;
			platformSpeed = currentPos.second;
			calculateSpeeds();
			soundManager->playSound(jumpSound);
			currentPosScore -= 1;
		}
		break;
	}
}

int Player::getScore() {
	return score;
}

Player::Player(){
}

Player::~Player()
{
}
