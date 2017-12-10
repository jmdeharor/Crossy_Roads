#pragma once
#include "GameObject.h"
#include "ShadowedObject.h"
#include "ImportedMesh.h"
#include "Floor.h"

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
	bool wPressed, aPressed, sPressed, dPressed, bPressed;
	bool inMovement;
	float speed, verticalSpeed, gravity;
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


	void setDirectionVector();
	bool keepMoving();
	float getJumpingSpeed(float y0, float y, glm::uint frames);
	void performRotation(Orientation currentOrientation, char key);
	bool collides();

public:
	void groupDrawableObjects(
		std::vector<std::vector<Object*>>& objects,
		std::vector<std::vector<TexturedObject*>>& texturedObjects, const FrustumG& frustum
	) override;
	void jump();
	void init(const Assets& assets, glm::vec3 lightDir, glm::vec3 offset, float jumpDistance, Floor &floor);
	PlayerReturn update(int deltaTime);
	void render(ShaderProgram& program);
	void renderShadow(ShaderProgram& program);
	glm::vec3 getPos() const;
	Player();
	~Player();
};

