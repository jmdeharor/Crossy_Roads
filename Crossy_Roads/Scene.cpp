#include <iostream>
#include <cmath>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include <GL/glut.h>

using namespace glm;

#define SHADOW_MAP_W 2048
#define SHADOW_MAP_H 2048

Scene::Scene() {
}

Scene::~Scene() {
}

void Scene::firstInit() {
	textScore.init("fonts/PiratesWriters.ttf");
	//assets.loadAssets("assets_locations/models.json", "assets_locations/textures.json");
	assets.loadAssets("binaryAssets.notxt");

	partSystem.init(assets);
	partSystem.g = -0.07f;

	renderVectors.objects.resize(assets.getNumMeshes());
	renderVectors.texturedObjects.resize(assets.getNumTextures());
	renderVectors.shadowObjects.resize(assets.getNumMeshes());

	soundManager = Game::instance().getSoundManager();
	music = soundManager->loadSound("sounds/Music_Caribbean_Smugglers.mp3", FMOD_LOOP_NORMAL | FMOD_CREATESTREAM);
	ambience = soundManager->loadSound("sounds/Ambiance_pirate_ship.mp3", FMOD_LOOP_NORMAL | FMOD_CREATESTREAM);

	QueryPerformanceFrequency(&frequency);

	initShaders();
	glEnable(GL_TEXTURE_2D);
	glStencilFunc(GL_EQUAL, 0, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);

	glGenFramebuffers(1, &framebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferName);
	glEnable(GL_DEPTH_TEST);
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_MAP_W, SHADOW_MAP_H, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);

	glDrawBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "Error with frame buffer" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glActiveTexture(GL_TEXTURE0);
}

inline void compileShader(ShaderProgram& program, const string& fileName) {
	Shader vShader, fShader;
	string path = "shaders/" + fileName;
	vShader.initFromFile(VERTEX_SHADER, path + ".vert");
	if (!vShader.isCompiled()) {
		cout << "Vertex Shader " + fileName +  " Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, path + ".frag");
	if (!fShader.isCompiled()) {
		cout << "Fragment Shader " + fileName + " Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	program.init();
	program.addShader(vShader);
	program.addShader(fShader);
	program.link();
	if (!program.isLinked()) {
		cout << "Shader " + fileName +  " Linking Error" << endl;
		cout << "" << program.log() << endl << endl;
	}
	vShader.free();
	fShader.free();
	for (uint i = 0; i < sizeof(uniformOrder) / sizeof(string); ++i) {
		program.addUniform(uniformOrder[i]);
	}
}

void Scene::initShaders() {
	compileShader(texProgram, "texture");
	texProgram.bindFragmentOutput("outColor");
	compileShader(lambertProgram, "directionalLight");
	lambertProgram.bindFragmentOutput("outColor");
	compileShader(shadowProgram, "shadow");
	shadowProgram.bindFragmentOutput("outColor");
	compileShader(shadowMapProgram, "shadowMap");
	shadowMapProgram.bindFragmentOutput("fragmentdepth");
	compileShader(drawShadowProgram, "drawShadow");
	drawShadowProgram.bindFragmentOutput("outColor");
	compileShader(drawImageProgram, "drawImage");
	drawImageProgram.bindFragmentOutput("outColor");
	compileShader(simple, "simple");
	simple.bindFragmentOutput("outColor");

	depthVPLoc = shadowMapProgram.addUniform("depthVP");

	drawShadowProgram.addUniform("depthVP");
	VPLoc = drawShadowProgram.addUniform("VP");

	drawShadowProgram.use();
	drawShadowProgram.setUniformi("tex", 0);
	drawShadowProgram.setUniformi("shadowMap", 1);
}

const uint rows = 5;
const uint cols = 5;

void Scene::init() {
	GameObject::init();

	lightDir = normalize(vec3(1,1,0));

	shadowMapProgram.use();
	shadowMapProgram.setUniform3f("lightDir", lightDir.x, lightDir.y, lightDir.z);
	drawShadowProgram.use();
	drawShadowProgram.setUniform3f("lightDir", lightDir.x, lightDir.y, lightDir.z);
	texProgram.use();
	texProgram.setUniform3f("lightDir", lightDir.x, lightDir.y, lightDir.z);

	floor.init(lightDir, assets, &player);
	player.init(assets, lightDir, vec3(0), floor.getTileSize().y, floor, &partSystem);
	camera.init(lightDir, &player);
	
	camera.updateVM();

	playerReferenceRow = playerRow = 0;

	FMOD::Channel* channel = soundManager->playSound(ambience);
	channel->setVolume(0.25f);

	soundManager->playSound(music);
}

SceneReturn Scene::update(int deltaTime) {
	QueryPerformanceCounter(&start);
	floor.update(deltaTime);
	camera.update(deltaTime);
	partSystem.update();
	PlayerReturn playerAction;
	if(playerControl)
		playerAction = player.update(deltaTime);
	else playerAction = PlayerReturn::NOTHING;

	switch (playerAction) {
	case PlayerReturn::NOTHING:
		break;
	case PlayerReturn::MOVE_FRONT:
		if (playerReferenceRow == playerRow) {
			floor.addLevel();
			++playerReferenceRow;
		}
		++playerRow;
		break;
	case PlayerReturn::MOVE_BACK:
		playerRow = playerRow - 1;
		break;
	case PlayerReturn::DEAD:
		return SceneReturn::EndGame;
		break;
	default:
		break;
	}
	if (Game::instance().getKey('p')) {
		int a = 3;
	}
	if (Game::instance().getKey('m')) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else if (Game::instance().getKey('n')) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (Game::instance().getKey('o')) {
		partSystem.trigger(player.getPos());
	}
	return SceneReturn::Nothing;
}

void Scene::render() {
	const mat4& viewProjection = *camera.getVPMatrix();
	const mat4& lightViewProjection = *camera.getVPLightMatrix();

	floor.groupDrawableObjects(camera.getFrustum(), renderVectors);
	player.groupDrawableObjects(camera.getFrustum(), renderVectors);

	sceneDrawCalls = 0;
	sceneTriangles = 0;

	glViewport(0, 0, SHADOW_MAP_W, SHADOW_MAP_H);
	shadowMapProgram.use();
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferName);
	glClear(GL_DEPTH_BUFFER_BIT);
	shadowMapProgram.setUniformMatrix4f(depthVPLoc, lightViewProjection);

	for (uint i = 0; i < renderVectors.shadowObjects.size(); ++i) {
		vector<Object*>& objects = renderVectors.shadowObjects[i];
		if (objects.empty())
			continue;
		const Mesh* mesh = assets.getMesh(i);
		mesh->setProgramParams(shadowMapProgram);
		for (uint j = 0; j < objects.size(); ++j) {
			Object* object = objects[j];
			shadowMapProgram.setUniformMatrix4f(modelLoc, *object->getModel());
			mesh->render();
		}
		objects.clear();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	const static mat4 offsetMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	drawShadowProgram.use();
	drawShadowProgram.setUniformMatrix4f(depthVPLoc, offsetMatrix*lightViewProjection);
	drawShadowProgram.setUniformMatrix4f(VPLoc, viewProjection);

	for (uint i = 0; i < renderVectors.objects.size(); ++i) {
		vector<Object*>& objects = renderVectors.objects[i];
		if (objects.empty())
			continue;
		const ImportedMesh* mesh = assets.getMesh(i);
		mesh->setProgramParams(drawShadowProgram);
		mesh->useTexture();
		for (uint j = 0; j < objects.size(); ++j) {
			Object* object = objects[j];
			drawShadowProgram.setUniformMatrix4f(modelLoc, *object->getModel());
			mesh->render();
		}
		objects.clear();
	}

	partSystem.render(drawShadowProgram);

	const Mesh* mesh = assets.getCubeMesh();
	mesh->setProgramParams(drawShadowProgram);
	for (uint i = 0; i < renderVectors.texturedObjects.size(); ++i) {
		vector<Object*>& objects = renderVectors.texturedObjects[i];
		if (objects.empty())
			continue;
		const Texture* tex = assets.getTexture(i);
		tex->use();
		for (uint j = 0; j < objects.size(); ++j) {
			Object* object = objects[j];
			drawShadowProgram.setUniformMatrix4f(modelLoc, *object->getModel());
			mesh->render();
		}
		objects.clear();
	}

	textScore.render(to_string(player.getScore()), vec2(SCREEN_WIDTH-50, 70), 32, vec4(1, 1, 1, 1));

	/*texProgram.use();
	texProgram.setUniformMatrix4f(projectionLoc, *camera.getProjectionMatrix());
	texProgram.setUniformMatrix4f(viewLoc, *camera.getViewMatrix());

	const Mesh* mesh = assets.getCubeMesh();
	mesh->setProgramParams(texProgram);
	for (uint i = 0; i < texturedObjects.size(); ++i) {
		vector<TexturedObject*>& objects = texturedObjects[i];
		const Texture* tex = assets.getTexture(i);
		tex->use();
		for (uint j = 0; j < objects.size(); ++j) {
			Object* object = objects[j];
			texProgram.setUniformMatrix4f(modelLoc, *object->getModel());
			mesh->render(texProgram);
		}
		objects.clear();
	}

	shadowProgram.use();
	shadowProgram.setUniformMatrix4f(projectionLoc, *camera.getProjectionMatrix());
	shadowProgram.setUniformMatrix4f(viewLoc, *camera.getViewMatrix());
	glEnable(GL_POLYGON_OFFSET_FILL);
	glEnable(GL_BLEND);
	glEnable(GL_STENCIL_TEST);
	glPolygonOffset(-1, -1);

	for (uint i = 0; i < objectsToRender.size(); ++i) {
		vector<Object*>& objects = objectsToRender[i];
		const Mesh* mesh = assets.getMesh(i);
		mesh->setProgramParams(shadowProgram);
		for (uint j = 0; j < objects.size(); ++j) {
			ShadowedObject* object = dynamic_cast<ShadowedObject*>(objects[j]);
			if (object != NULL) {
				shadowProgram.setUniformMatrix4f(modelLoc, object->getShadowModel());
				mesh->render(shadowProgram);
			}
		}
		objects.clear();
	}

	glDisable(GL_STENCIL_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_POLYGON_OFFSET_FILL);*/

	QueryPerformanceCounter(&end);
	LARGE_INTEGER elapsed;
	elapsed.QuadPart = ((end.QuadPart - start.QuadPart) * 1000000) / frequency.QuadPart;
	cout << elapsed.QuadPart / (double)1000 << endl;
	cout << sceneTriangles << " " << sceneDrawCalls << endl;
}

void Scene::resize(int w, int h) {
	camera.resize(w, h);
}

uint Scene::sceneTriangles = 0;
uint Scene::sceneDrawCalls = 0;