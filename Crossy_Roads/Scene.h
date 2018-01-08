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
#include "ParticleSystem.h"

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

	void enablePlayerControl();
	void disablePlayerControl();
private:
	void firstInit() override;
	void initShaders();

private:
	LARGE_INTEGER start, end;
	LARGE_INTEGER frequency;
	glm::uint depthVPLoc, deadLoc;
	int playerReferenceRow, playerRow;
	bool playerDead;

	Assets assets;
	RenderVectors renderVectors;
	WaterParticleSystem partSystem;
	ParticleSystem coinPartSystem;
	const SoundManager* soundManager;
	FMOD::Sound* music, *ambience;

	Text textScore;
	Text textCoins;
	Player player;
	Floor floor;
	GLuint framebufferName, depthTexture;
	glm::vec3 lightDir;
	Camera camera;

	ShaderProgram shadowMapDepth;
	ShaderProgram shadowMapDraw;
	ShaderProgram drawShadowProjection;
};


#endif // _SCENE_INCLUDE

