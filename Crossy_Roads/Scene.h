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
#include "Text.h"

#define CAMERA_WIDTH 640
#define CAMERA_HEIGHT 480

enum class SceneReturn {
	EndGame,
	Nothing
};

// Scene contains all the entities of our game.
// It is responsible for updating and render them.

class Scene: public GameObject {

public:
	static glm::uint sceneTriangles;
	static glm::uint sceneDrawCalls;

	Scene();
	~Scene();

	void init();
	SceneReturn update(int deltaTime);
	void render();
	void resize(int w, int h);

	bool playerControl;
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
	const SoundManager* soundManager;
	FMOD::Sound* music, *ambience;

	Text textScore;

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

