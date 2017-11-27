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

void Scene::firstInit() {
	initShaders();
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glStencilFunc(GL_EQUAL, 0, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
	pirateMesh.loadFromFile("models/pirate.obj");

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
}

const uint rows = 5;
const uint cols = 5;

void Scene::init() {
	GameObject::init();

	lightDir = normalize(vec3(1,1,0.2f));

	floor.init(lightDir);
	camera.init(lightDir, &player);
	player.init(lightDir, vec3(0), floor.getTileSize().y);

	camera.setPos(player.getPos());
	camera.updateVM();
	pressed = false;
}

void Scene::update(int deltaTime) {
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
	const static mat4 offsetMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);

	glViewport(0, 0, SHADOW_MAP_W, SHADOW_MAP_H);
	shadowMapProgram.use();
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferName);
	glClear(GL_DEPTH_BUFFER_BIT);
	shadowMapProgram.setUniformMatrix4f("depthVP", camera.getVPLightMatrix());
	shadowMapProgram.setUniform3f("lightDir", lightDir.x, lightDir.y, lightDir.z);

	floor.renderLightObjects(shadowMapProgram);
	player.render(shadowMapProgram);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glActiveTexture(GL_TEXTURE0);
	drawShadowProgram.use();
	drawShadowProgram.setUniformi("tex", 0);
	drawShadowProgram.setUniformi("shadowMap", 1);
	drawShadowProgram.setUniformMatrix4f("depthVP", offsetMatrix*camera.getVPLightMatrix());
	drawShadowProgram.setUniformMatrix4f("VP", camera.getVPMatrix());
	drawShadowProgram.setUniform3f("lightDir", lightDir.x, lightDir.y, lightDir.z);

	//floor.renderSimpleObjects(drawShadowProgram);
	floor.renderLightObjects(drawShadowProgram);
	player.render(drawShadowProgram);

	texProgram.use();
	texProgram.setUniformMatrix4f(projectionLoc, *camera.getProjectionMatrix());
	texProgram.setUniformMatrix4f(viewLoc, *camera.getViewMatrix());
	texProgram.setUniform3f("lightDir", lightDir.x, lightDir.y, lightDir.z);
	texProgram.setUniformMatrix3f(normalMatrixLoc, mat3(*camera.getViewMatrix()));

	floor.renderSimpleObjects(texProgram);

	shadowProgram.use();
	shadowProgram.setUniformMatrix4f(projectionLoc, *camera.getProjectionMatrix());
	shadowProgram.setUniformMatrix4f(viewLoc, *camera.getViewMatrix());
	glEnable(GL_POLYGON_OFFSET_FILL);
	glEnable(GL_BLEND);
	glEnable(GL_STENCIL_TEST);
	glPolygonOffset(-1, -1);

	floor.renderShadows(shadowProgram);
	player.renderShadow(shadowProgram);

	glDisable(GL_STENCIL_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_POLYGON_OFFSET_FILL);
}

void Scene::resize(int w, int h) {
	camera.resize(w, h);
}