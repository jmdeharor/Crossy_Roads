#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Level.h"
#include "Camera.h"
#include "Sphere.h"
#include "ShadowedObject.h"
#include "GameObject.h"

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
	void initShaders();
	void firstInit();

private:
	glm::vec3 lightDir;
	glm::vec4 lightAmbient;
	glm::vec4 lightDiffuse;
	Camera camera;
	Level *level;
	Mesh pirateMesh;
	std::vector<ShadowedObject> pirates;
	ShaderProgram texProgram, lambertProgram, shadowProgram;
	float currentTime;
};


#endif // _SCENE_INCLUDE

