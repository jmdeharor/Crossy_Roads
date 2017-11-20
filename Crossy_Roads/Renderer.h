#pragma once
#include <vector>
#include "ShadowedObject.h"
#include "GameObject.h"
#include "Camera.h"
#include "glm\glm.hpp"

class Renderer : public GameObject {
	std::vector<Object*> simpleObjects;
	std::vector<Object*> objects;
	std::vector<ShadowedObject*> shadowedObjects;
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
	void addGroup(Object* objects, uint size);
	void addSimpleGroup(Object* objects, uint size);
	void render();
	void update(int deltaTime);
	Renderer();
	~Renderer();
};

