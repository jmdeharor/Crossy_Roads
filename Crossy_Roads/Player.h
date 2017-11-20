#pragma once
#include "GameObject.h"
#include "Mesh.h"
#include "ShadowedObject.h"

class Player : public GameObject {
	Mesh playerMesh;
	ShadowedObject playerObject;
	void firstInit();
public:
	void jump();
	void init();
	void update(int deltaTime);
	void render(ShaderProgram& program);
	void renderShadow(ShaderProgram& program);
	Player();
	~Player();
};

