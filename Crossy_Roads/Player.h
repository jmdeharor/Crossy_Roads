#pragma once
#include "GameObject.h"
#include "ShadowedObject.h"
#include "ImportedMesh.h"

enum Orientation {
	FRONT,
	LEFT,
	BACK,
	RIGHT
};

class Player : public GameObject {
	ImportedMesh playerMesh;
	ShadowedObject playerObject;
	void firstInit();
private:
	bool frontPressed;
	bool leftPressed;
	bool backPressed;
	bool rightPressed;
	float jumpDistance;
	glm::vec3 directionVector;
	Orientation currentOrientation;

	void setDirectionVector();

public:
	void jump();
	void init(glm::vec3 lightDir, glm::vec3 offset, float jumpDistance);
	bool update(int deltaTime);
	void render(ShaderProgram& program);
	void renderShadow(ShaderProgram& program);
	glm::vec3 getPos();
	Player();
	~Player();
};

