#pragma once
#include <vector>
#include "ShadowedObject.h"
#include "GameObject.h"
#include "Camera.h"
#include "glm\glm.hpp"

typedef unsigned char byte;

struct ObjectVector {
	byte* vector;
	uint size, stride;
};

class Renderer : public GameObject {
	std::vector<ObjectVector> simpleObjects;
	std::vector<ObjectVector> objects;
	std::vector<ObjectVector> shadowedObjects;
	ShaderProgram texProgram, lambertProgram, shadowProgram;
	ShaderProgram shadowMapProgram, drawShadowProgram, drawImageProgram;
	Camera* camera;
	glm::vec3 lightDir;
	glm::vec4 lightAmbient;
	glm::vec4 lightDiffuse;

	void initShaders();
	void firstInit();
public:
	void init();
	void setLightParameters(glm::vec3 lightDir, glm::vec4 lightAmbient, glm::vec4 lightDiffuse);
	void setCamera(Camera* camera);
	void addGroup(Object* objects, uint size, uint stride);
	void addSimpleGroup(Object* objects, uint size, uint stride);
	void render();
	void update(int deltaTime);
	Renderer();
	~Renderer();
};

