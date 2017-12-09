#include <iostream>
#include <cmath>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
using namespace glm;

#define SHADOW_MAP_W 2048
#define SHADOW_MAP_H 2048

Scene::Scene() {
}

Scene::~Scene() {
}

const string meshNames[] = { 
	"pirate", "pirate_2",
	"barrel", "box",
	"cubierta"
};

const string textureNames[] = { 
	"wood_3_1", "wood_3_2", "wood_3_3", "wood_3_4", "wood_3_5",
	"wood_4_0", "wood_4_1", "wood_4_2", "wood_4_3", "wood_4_4",
	"wood_5_0", "wood_5_1", "wood_5_2", "wood_5_3", "wood_5_4",
	"wood_6_0", "wood_6_1", "wood_6_2", "wood_6_3", "wood_6_4",
	"wood_7_0", "wood_7_1", "wood_7_2", "wood_7_3", "wood_7_4",
	"wood_plane"
};

const TextureFilter textureMode[] = {
	LINEAR, LINEAR, LINEAR, LINEAR, LINEAR,
	LINEAR, LINEAR, LINEAR, LINEAR, LINEAR,
	LINEAR, LINEAR, LINEAR, LINEAR, LINEAR,
	LINEAR, LINEAR, LINEAR, LINEAR, LINEAR,
	LINEAR, LINEAR, LINEAR, LINEAR, LINEAR,
	NEAREST
};

const uint nMeshes = sizeof(meshNames) / sizeof(string);
const uint nTextures = sizeof(textureNames) / sizeof(string);

void Scene::firstInit() {
	assets.loadAssets(meshNames, textureNames, textureMode, nMeshes, nTextures);

	objectsToRender.resize(nMeshes);
	texturedObjects.resize(nTextures);

	QueryPerformanceFrequency(&frequency);

	initShaders();
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

	depthVPLoc1 = shadowMapProgram.addUniform("depthVP");
	VPLoc = drawShadowProgram.addUniform("VP");

	depthVPLoc2 = drawShadowProgram.addUniform("depthVP");

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

	floor.init(lightDir, assets);
	player.init(assets, lightDir, vec3(0), floor.getTileSize().y, floor);
	camera.init(lightDir, &player);
	
	camera.updateVM();
}

void Scene::update(int deltaTime) {
	QueryPerformanceCounter(&start);
	camera.update(deltaTime);
	floor.update(deltaTime);
	PlayerReturn playerAction;
	playerAction = player.update(deltaTime);
	switch (playerAction) {
	case PlayerReturn::NOTHING:
		break;
	case PlayerReturn::MOVE_FRONT:
		floor.addLevel();
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
}

void Scene::render() {
	const mat4& viewProjection = *camera.getVPMatrix();
	const mat4& lightViewProjection = *camera.getVPLightMatrix();

	floor.groupDrawableObjects(objectsToRender, texturedObjects, camera.getFrustum());
	player.groupDrawableObjects(objectsToRender, texturedObjects, camera.getFrustum());

	sceneDrawCalls = 0;
	sceneTriangles = 0;

	glViewport(0, 0, SHADOW_MAP_W, SHADOW_MAP_H);
	shadowMapProgram.use();
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferName);
	glClear(GL_DEPTH_BUFFER_BIT);
	shadowMapProgram.setUniformMatrix4f(depthVPLoc1, lightViewProjection);

	for (uint i = 0; i < objectsToRender.size(); ++i) {
		vector<Object*>& objects = objectsToRender[i];
		const Mesh* mesh = assets.getMesh(i);
		mesh->setProgramParams(shadowMapProgram);
		for (uint j = 0; j < objects.size(); ++j) {
			Object* object = objects[j];
			shadowMapProgram.setUniformMatrix4f(modelLoc, *object->getModel());
			mesh->render(shadowMapProgram);
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	const static mat4 offsetMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glActiveTexture(GL_TEXTURE0);
	drawShadowProgram.use();
	drawShadowProgram.setUniformMatrix4f(depthVPLoc2, offsetMatrix*lightViewProjection);
	drawShadowProgram.setUniformMatrix4f(VPLoc, viewProjection);

	for (uint i = 0; i < objectsToRender.size(); ++i) {
		vector<Object*>& objects = objectsToRender[i];
		const ImportedMesh* mesh = assets.getMesh(i);
		mesh->setProgramParams(drawShadowProgram);
		mesh->useTexture();
		for (uint j = 0; j < objects.size(); ++j) {
			Object* object = objects[j];
			drawShadowProgram.setUniformMatrix4f(modelLoc, *object->getModel());
			mesh->render(drawShadowProgram);
		}
	}

	texProgram.use();
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
	glDisable(GL_POLYGON_OFFSET_FILL);

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