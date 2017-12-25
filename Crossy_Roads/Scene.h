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
#include "ImportedMesh.h"
#include <Windows.h>
#include "MultiCubeMesh.h"
#include "Assets.h"
#include "FrustumG.h"
#include "ObjectContainer.h"
#include "WaterParticles.h"
#include "SoundManager.h"

#define CAMERA_WIDTH 640
#define CAMERA_HEIGHT 480


// Scene contains all the entities of our game.
// It is responsible for updating and render them.

class Scene: public GameObject {

public:
	static glm::uint sceneTriangles;
	static glm::uint sceneDrawCalls;

	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();
	void resize(int w, int h);

private:
	void firstInit() override;
	void initShaders();

private:
	LARGE_INTEGER start, end;
	LARGE_INTEGER frequency;
	glm::uint depthVPLoc, VPLoc;
	int playerReferenceRow, playerRow;

	Assets assets;
	RenderVectors renderVectors;
	WaterParticleSystem partSystem;
	SoundManager soundManager;
	FMOD::Sound* sound;

	Player player;
	Floor floor;
	GLuint framebufferName, depthTexture;
	glm::vec3 lightDir;
	Camera camera;
	ShaderProgram texProgram, lambertProgram, shadowProgram;
	ShaderProgram shadowMapProgram, drawShadowProgram, drawImageProgram;
	ShaderProgram simple;
};


#endif // _SCENE_INCLUDE

