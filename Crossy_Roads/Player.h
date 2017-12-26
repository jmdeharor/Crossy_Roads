#pragma once
#include "GameObject.h"
#include "ShadowedObject.h"
#include "ImportedMesh.h"
#include "Floor.h"
#include "SoundManager.h"

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
	NOTHING
};

class Player : public GameObject, public ObjectContainer {
	ShadowedObject playerObject;
	void firstInit() override;
private:
	float platformSpeed;
	bool wPressed, aPressed, sPressed, dPressed, bPressed;
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
	bool upsideDown;
	glm::vec3 nextPos;
	SoundManager soundManager;
	FMOD::Sound* jumpSound;

	void setDirectionVector();
	bool keepMoving();
	float getJumpingSpeed(float y0, float y, glm::uint frames);
	void performRotation(char key);
	bool collides();
	bool collidesWithEnv(glm::uint row, glm::uint col);

public:
	void groupDrawableObjects(
		const FrustumG& frustum, RenderVectors& renderVectors
	) override;
	void jump();
	void init(const Assets& assets, glm::vec3 lightDir, glm::vec3 offset, float jumpDistance, Floor &floor);
	PlayerReturn update(int deltaTime);
	glm::vec3 getPos() const;
	float getHeight() const;
	void calculateSpeeds();
	Player();
	~Player();
};

