#include <iostream>
#include <cmath>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
using namespace glm;

#define PI 3.14159f

Scene::Scene() {
	level = NULL;
}

Scene::~Scene() {
	if(level != NULL)
		delete level;
}

void Scene::firstInit() {
	initShaders();
	pirateMesh.loadFromFile("models/pirate.obj", lambertProgram);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glStencilFunc(GL_EQUAL, 0, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
}

const uint rows = 5;
const uint cols = 5;

void Scene::init() {
	GameObject::init();
	level = Level::createLevel(vec3(16, 4, 32), texProgram, "images/floor.png", "images/wall.png");
	currentTime = 0.0f;
	camera.init();

	lightAmbient = vec4(0.3f);
	lightDiffuse = vec4(0.8f);
	lightPos = vec4(0, 1, 1, 1);
	pirates.resize(rows*cols);
	vec3 offset = vec3(-6, -2, -6);
	for (uint i = 0; i < rows; ++i) {
		for (uint j = 0; j < cols; ++j) {
			ShadowedObject& pirate = pirates[i*cols + j];
			pirate.setMesh(&pirateMesh);
			pirate.setShader(&lambertProgram);
			pirate.setShadowShader(&shadowProgram);

			pirate.setScale(vec3(0.1f));
			pirate.setPos(vec3(offset.x + i*3, offset.y + pirate.getHeight() / 2, offset.z + j*3));
			pirate.setPlane(vec4(0, 1, 0, -offset.y), vec3(1, 1, 0));
			pirate.updateModel();
		}
	}
	camera.setPos(vec3(0));
	camera.updateVM();
}

void Scene::update(int deltaTime) {
	camera.update(deltaTime);
	bool modified = false;
	/*if (Game::instance().getKey('a')) {
		pirate.move(-0.1f, 0, 0);
		modified = true;
	}
	if (Game::instance().getKey('d')) {
		pirate.move(0.1f, 0, 0);
		modified = true;
	}
	if (Game::instance().getKey('w')) {
		pirate.move(0, 0, 0.1f);
		modified = true;
	}
	if (Game::instance().getKey('s')) {
		pirate.move(0, 0, -0.1f);
		modified = true;
	}*/
	if (Game::instance().getKey('q')) {
		for (uint i = 0; i < pirates.size(); ++i) {
			pirates[i].rotateY(0.1f);
		}
		modified = true;
	}
	if (Game::instance().getKey('e')) {
		for (uint i = 0; i < pirates.size(); ++i) {
			pirates[i].rotateY(-0.1f);
		}
		modified = true;
	}
	if (modified) {
		for (uint i = 0; i < pirates.size(); ++i) {
			pirates[i].updateModel();
		}
		camera.updateVM();
	}
}

void Scene::render() {
	texProgram.use();
	texProgram.setUniformMatrix4f((uint)Location::projection, *camera.getProjectionMatrix());
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	texProgram.setUniformMatrix4f("modelview", *camera.getViewMatrix());
	level->render();

	lambertProgram.use();
	lambertProgram.setUniformMatrix4f((uint)Location::projection, *camera.getProjectionMatrix());
	lambertProgram.setUniformMatrix4f((uint)Location::view, *camera.getViewMatrix());
	vec4 realLightPos = (*camera.getViewMatrix())*lightPos;
	lambertProgram.setUniform4f("lightAmbient", lightAmbient);
	lambertProgram.setUniform4f("lightDiffuse", lightDiffuse);
	lambertProgram.setUniform4f("lightPosition", realLightPos);
	lambertProgram.setUniform4f("matDiffuse", 1, 1, 1, 1);
	lambertProgram.setUniform4f("matAmbient", 1, 1, 1, 1);
	for (uint i = 0; i < pirates.size(); ++i) {
		pirates[i].render();
	}

	shadowProgram.use();
	shadowProgram.setUniformMatrix4f((uint)Location::projection, *camera.getProjectionMatrix());
	shadowProgram.setUniformMatrix4f((uint)Location::view, *camera.getViewMatrix());
	glEnable(GL_POLYGON_OFFSET_FILL);
	glEnable(GL_BLEND);
	glEnable(GL_STENCIL_TEST);

	glPolygonOffset(-1, -1);
	for (uint i = 0; i < pirates.size(); ++i) {
		pirates[i].renderShadow();
	}
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_POLYGON_OFFSET_FILL);
}

void Scene::resize(int w, int h) {
	camera.resize(w, h);
}

inline void compileShader(ShaderProgram& program, const string& fileName) {
	Shader vShader, fShader;
	string path = "shaders/" + fileName;
	vShader.initFromFile(VERTEX_SHADER, path + ".vert");
	if (!vShader.isCompiled()) {
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, path + ".frag");
	if (!fShader.isCompiled()) {
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	program.init();
	program.addShader(vShader);
	program.addShader(fShader);
	program.link();
	if (!program.isLinked()) {
		cout << "Shader Linking Error" << endl;
		cout << "" << program.log() << endl << endl;
	}
	program.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();

	for (uint i = 0; i < sizeof(uniformOrder) / sizeof(string); ++i) {
		program.addUniform(uniformOrder[i]);
	}
}

void Scene::initShaders() {
	compileShader(texProgram, "texture");
	compileShader(lambertProgram, "lambert");
	compileShader(shadowProgram, "shadow");
}