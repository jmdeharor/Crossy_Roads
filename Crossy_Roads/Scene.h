#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Camera.h"
#include "Sphere.h"
#include "QuadMesh.h"
#include "ShadowedObject.h"
#include "GameObject.h"
#include "Floor.h"
#include "Player.h"
#include "Renderer.h"
#include "ImportedMesh.h"

#define CAMERA_WIDTH 640
#define CAMERA_HEIGHT 480


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene: public GameObject {

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();
	void resize(int w, int h);

private:
	void firstInit();
	void initShaders();

private:
	Player player;
	ShadowedObject* enemies;
	Floor floor;
	ImportedMesh pirateMesh;
	GLuint framebufferName, depthTexture;
	glm::vec3 lightDir;
	glm::vec4 lightAmbient;
	glm::vec4 lightDiffuse;
	Camera camera;
	bool pressed;
	float currentTime;
	ShaderProgram texProgram, lambertProgram, shadowProgram;
	ShaderProgram shadowMapProgram, drawShadowProgram, drawImageProgram;
};


#endif // _SCENE_INCLUDE

