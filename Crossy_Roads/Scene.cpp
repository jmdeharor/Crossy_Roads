#include <iostream>
#include <cmath>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
using namespace glm;

#define PI 3.14159f
#define SHADOW_MAP_W 4096
#define SHADOW_MAP_H 4096

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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);

	glDrawBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "Error with frame buffer" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	quad.init();
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
	currentTime = 0.0f;
	lightDir = normalize(vec3(1,1,0));
	floor.setLight(lightDir);
	floor.init();
	lightAmbient = vec4(0.15f);
	lightDiffuse = vec4(0.85f);
	vec3 offset = vec3(0, 0, 0);

	camera.init(lightDir);
	player.init(lightDir, offset, floor.getTileSize().y);
	camera.setPos(player.getPos());
	camera.updateVM();
	pressed = false;
}

void Scene::update(int deltaTime) {
	camera.update(deltaTime);
	floor.update(deltaTime);
	bool modified = false;
	modified = player.update(deltaTime);
	if (Game::instance().getKey('p')) {
		int a = 3;
	}
	if (modified) {
		camera.setPos(player.getPos());
		camera.updateVM();
	}
	if (Game::instance().getKey('m')) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else if (Game::instance().getKey('n')) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
}

void Scene::render() {
	const static mat4 biasMatrix(
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
	drawShadowProgram.setUniformMatrix4f("depthVP", biasMatrix*camera.getVPLightMatrix());
	drawShadowProgram.setUniformMatrix4f("VP", camera.getVPMatrix());

	floor.renderSimpleObjects(drawShadowProgram);
	floor.renderLightObjects(drawShadowProgram);
	player.render(drawShadowProgram);

	/*texProgram.use();
	texProgram.setUniformMatrix4f(projectionLoc, *camera.getProjectionMatrix());
	texProgram.setUniformMatrix4f(viewLoc, *camera.getViewMatrix());
	texProgram.setUniform4f("color", vec4(1));
	floor.renderSimpleObjects(texProgram);

	lambertProgram.use();
	GLint loc = glGetUniformLocation(lambertProgram.getProgramId(), "shadowMap");
	glUniform1i(loc, 1); // set it manually
	loc = glGetUniformLocation(lambertProgram.getProgramId(), "tex");
	glUniform1i(loc, 0); // set it manually
	mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);
	lambertProgram.setUniformMatrix4f("depthVP", biasMatrix*depthMVP);
	lambertProgram.setUniformMatrix4f(projectionLoc, *camera.getProjectionMatrix());
	lambertProgram.setUniformMatrix4f(viewLoc, *camera.getViewMatrix());
	vec3 lightD = mat3(*camera.getViewMatrix())*lightDir;
	lambertProgram.setUniform4f("lightAmbient", lightAmbient);
	lambertProgram.setUniform4f("lightDiffuse", lightDiffuse);
	lambertProgram.setUniform3f("lightDirection", lightD.x, lightD.y, lightD.z);
	lambertProgram.setUniform4f("matDiffuse", 1, 1, 1, 1);
	lambertProgram.setUniform4f("matAmbient", 1, 1, 1, 1);
	floor.renderLightObjects(lambertProgram);
	player.render(lambertProgram);

	glDisable(GL_TEXTURE_2D);

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

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	drawShadowProgram.use();
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glActiveTexture(GL_TEXTURE0);
	mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);
	GLint loc = glGetUniformLocation(drawShadowProgram.getProgramId(), "shadowMap");
	glUniform1i(loc, 1); // set it manually
	drawShadowProgram.setUniformMatrix4f("VP", (*camera.getProjectionMatrix())*(*camera.getViewMatrix()));
	drawShadowProgram.setUniformMatrix4f("depthVP", biasMatrix*depthMVP);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glEnable(GL_BLEND);
	glEnable(GL_STENCIL_TEST);
	glPolygonOffset(-1, -1);

	floor.renderSimpleObjects(shadowMapProgram);
	floor.renderLightObjects(shadowMapProgram);
	player.render(shadowMapProgram);


	glDisable(GL_STENCIL_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_POLYGON_OFFSET_FILL);*/

	/*glViewport(0, 0, 200, 200);
	drawImageProgram.use();
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	quad.render(drawImageProgram);*/
}

void Scene::resize(int w, int h) {
	camera.resize(w, h);
}