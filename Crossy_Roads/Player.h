#pragma once
#include "GameObject.h"
#include "ShadowedObject.h"
#include "ImportedMesh.h"
#include "Floor.h"
#include "WaterParticles.h"
#include "SoundManager.h"

class Game;

enum Orientation {
	FRONT,
	LEFT,
	BACK,
	RIGHT
};

enum class PlayerReturn {
	MOVE_FRONT,
	MOVE_BACK,
	MOVE_LEFT,
	MOVE_RIGHT,
	DEAD,
	NOTHING
};

enum class PlayerState {
	Alive,
	DeadByLava,
	DeadByOut,
	DeadByEnemy
};

class Player : public GameObject, public ObjectContainer {
	Game* game;
	ShadowedObject playerObject;
	TexturedObject textureObject;
	PlayerState state;
	const std::vector<IdMesh>* playerModels;
	bool outOfTheScene;
	WaterParticleSystem* particleSystem;
	int currentPosScore;
	int score;
	float platformSpeed;
	bool wPressed, aPressed, sPressed, dPressed;
	bool mousePressed;
	bool inMovement;
	float speed, verticalSpeed, gravity;
	glm::vec3 speeds;
	float currentVerticalSpeed;
	float jumpDistance;
	float testJump;
	glm::uint currentFrame;
	glm::vec3 directionVector;
	Orientation currentOrientation;
	Floor *floor;
	glm::uint currentRowIndex;
	glm::uint currentColIndex;
	glm::vec3 lightDir;
	glm::vec3 nextPos;
	FMOD::Sound* jumpSound, *waterSplashSound, *deathCrush, *deathOut;
	const SoundManager* soundManager;
	const Assets* assets;
	bool godModePressed, godMode;
	bool playerControl;
	int charSelected;
private:
	void firstInit() override;
	void setDirectionVector();
	bool keepMoving();
	float getJumpingSpeed(float y0, float y, glm::uint frames);
	void performRotation(char key);
	bool collides();
	bool collidesWithEnv(glm::uint row, glm::uint col);
	void move(Orientation orientation);
public:
	void groupDrawableObjects(
		const FrustumG& frustum, RenderVectors& renderVectors
	) override;
	void init(const Assets& assets, glm::vec3 lightDir, glm::vec3 offset, float jumpDistance, Floor& floor, WaterParticleSystem* particleSystem);
	PlayerReturn update(int deltaTime);
	glm::vec3 getPos() const;
	float getHeight() const;
	const Object* getObject() const;
	void calculateSpeeds();
	void disablePlayerControl();
	void enablePlayerControl();
	int getScore();
	Player();
	~Player();
};

