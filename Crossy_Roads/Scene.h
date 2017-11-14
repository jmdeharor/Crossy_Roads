#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Level.h"
#include "Camera.h"
#include "Sphere.h"
#include "Object.h"
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

private:
	void initShaders();

private:
	Camera camera;
	Level *level;
	Object sphere1, sphere2;
	Sphere sphereMesh;
	ShaderProgram texProgram, simpleProgram;
	float currentTime;
};


#endif // _SCENE_INCLUDE

