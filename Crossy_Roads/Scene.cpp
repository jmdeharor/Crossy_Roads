#include <iostream>
#include <cmath>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
using namespace glm;

#define PI 3.14159f


Scene::Scene()
{
	level = NULL;
}

Scene::~Scene()
{
	if(level != NULL)
		delete level;
}

void Scene::init() {
	initShaders();
	level = Level::createLevel(vec3(16, 4, 32), texProgram, "images/floor.png", "images/wall.png");
	currentTime = 0.0f;
	camera.init();
	mesh.init(101, 100, 2, simpleProgram);
}

void Scene::update(int deltaTime) {
	currentTime += deltaTime;
	camera.update(deltaTime);
}

void Scene::render() {
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", *camera.getProjectionMatrix());
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	texProgram.setUniformMatrix4f("modelview", *camera.getViewMatrix());
	level->render();
	simpleProgram.use();
	simpleProgram.setUniformMatrix3f("normalMatrix", inverse(transpose(mat3(*camera.getViewMatrix()))));
	simpleProgram.setUniformMatrix4f("projection", *camera.getProjectionMatrix());
	simpleProgram.setUniformMatrix4f("modelview", *camera.getViewMatrix());
	mesh.render();
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();


	vShader.initFromFile(VERTEX_SHADER, "shaders/simple.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/simple.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	simpleProgram.init();
	simpleProgram.addShader(vShader);
	simpleProgram.addShader(fShader);
	simpleProgram.link();
	if (!simpleProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << simpleProgram.log() << endl << endl;
	}
	simpleProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}