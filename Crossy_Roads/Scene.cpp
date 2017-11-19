#include <iostream>
#include <cmath>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
using namespace glm;

#define PI 3.14159f

Scene::Scene() {
}

Scene::~Scene() {
}

void Scene::firstInit() {
	pirateMesh.loadFromFile("models/pirate.obj");

	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	glGenFramebuffers(1, &framebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferName);
	glEnable(GL_DEPTH_TEST);
	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);

	glDrawBuffer(GL_NONE); // No color buffer is drawn to.

	// Always check that our framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "Error with frame buffer" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	static const GLfloat g_quad_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f,  1.0f, 0.0f,
	};

	glGenVertexArrays(1, &boxVao);
	glBindVertexArray(boxVao);

	glGenBuffers(1, &boxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, boxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
}

const uint rows = 5;
const uint cols = 5;

void Scene::init() {
	GameObject::init();
	currentTime = 0.0f;
	renderer.init();
	camera.init();
	lightDir = vec3(1,1,0);
	floor.setLight(lightDir);
	floor.init();
	lightAmbient = vec4(0.15f);
	lightDiffuse = vec4(0.85f);
	vec2 tileSize = floor.getTileSize();
	vec3 offset = vec3(0, 0, 0);
	pirate.setMesh(&pirateMesh);
	pirate.name = "player";

	pirate.setScale(vec3(0.1f));
	pirate.setPos(vec3(0, pirate.getHeight() / 2, 0)+offset);
	pirate.setPlane(vec4(0, 1, 0, -offset.y), lightDir);
	pirate.updateModel();

	camera.setPos(pirate.getPos());
	camera.updateVM();
	pressed = false;

	floor.addObjects(renderer);
	renderer.addGroup(&pirate, 1, sizeof(pirate));
	renderer.setCamera(&camera);
	renderer.setLightParameters(lightDir, lightAmbient, lightDiffuse);
}

void Scene::update(int deltaTime) {
	camera.update(deltaTime);
	renderer.update(deltaTime);
	floor.update(deltaTime);
	bool modified = false;
	if (Game::instance().getKey('w')) {
		if (!pressed) {
			pressed = true;
			pirate.move(0, 0, floor.getTileSize().y);
			floor.addLevel();
			modified = true;
		}
	}
	else
		pressed = false;
	if (Game::instance().getKey('q')) {
		pirate.rotateY(0.1f);
		modified = true;
	}
	if (Game::instance().getKey('e')) {
		pirate.rotateY(-0.1f);
		modified = true;
	}
	if (Game::instance().getKey('p')) {
		int a = 3;
	}
	if (modified) {
		camera.setPos(pirate.getPos());
		pirate.updateModel();
		camera.updateVM();
	}
}

void Scene::render() {
	renderer.render();

	/*glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 20);
	glm::mat4 depthViewMatrix = glm::lookAt(vec3(0, 5, 0), glm::vec3(0, 0, 0), glm::vec3(1, 0, 0));
	glm::mat4 depthModelMatrix = mat4(1);
	glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;

	glEnable(GL_TEXTURE_2D);
	glViewport(0, 0, 1024, 1024);
	shadowMapProgram.use();
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferName);
	glClear(GL_DEPTH_BUFFER_BIT);
	shadowMapProgram.setUniformMatrix4f("depthVP", depthMVP);
	for (uint i = 0; i < pirates.size(); ++i) {
		pirates[i].render(shadowMapProgram);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	drawShadowProgram.use();
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);
	drawShadowProgram.setUniformMatrix4f("VP", (*camera.getProjectionMatrix())*(*camera.getViewMatrix()));
	drawShadowProgram.setUniformMatrix4f("depthVP", depthMVP);
	drawShadowProgram.setUniformMatrix4f("bias", biasMatrix);
	for (uint i = 0; i < pirates.size(); ++i) {
		pirates[i].render(drawShadowProgram);
	}

	glViewport(0, 0, 200, 200);
	drawImageProgram.use();
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glBindVertexArray(boxVao);
	glBindBuffer(GL_ARRAY_BUFFER, boxVBO);
	glEnableVertexAttribArray(positionLoc);
	drawImageProgram.bindVertexAttribute(positionLoc, 3, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);*/
}

void Scene::resize(int w, int h) {
	camera.resize(w, h);
}