#include <iostream>
#include <cmath>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
using namespace glm;

#define PI 3.14159f

Scene::Scene() {
	level = NULL;
}

Scene::~Scene() {
	if(level != NULL)
		delete level;
}

void Scene::init() {
	initShaders();
	level = Level::createLevel(vec3(16, 4, 32), texProgram, "images/floor.png", "images/wall.png");
	currentTime = 0.0f;
	camera.init();
	/*sphereMesh.initMore(50, 50, 2, simpleProgram);
	sphere1.setMesh(&sphereMesh);
	sphere2.setMesh(&sphereMesh);
	lightSphere.setMesh(&sphereMesh);

	sphere1.setPos(vec3(0));
	sphere2.setPos(vec3(0, 0, -3));
	sphere1.updateModel();
	sphere2.updateModel();*/
	lightAmbient = vec4(0.3f);
	lightDiffuse = vec4(0.8f);
	lightPos = vec4(0, 5, 0, 1);

	/*lightSphere.setPos(vec3(lightPos));
	lightSphere.setScale(vec3(0.1f));
	lightSphere.updateModel();*/

	projectionLoc = simpleProgram.addUniform("projection");

	//boxModel.loadFromFile("models/room.obj", simpleProgram);
	pirateMesh.loadFromFile("models/pirate.obj", simpleProgram);
	pirate.setMesh(&pirateMesh);
	pirate.setScale(vec3(0.1));
	pirate.updateModel();
}

void Scene::update(int deltaTime) {
	camera.update(deltaTime);
}

void Scene::render() {
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", *camera.getProjectionMatrix());
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	texProgram.setUniformMatrix4f("modelview", *camera.getViewMatrix());
	level->render();
	simpleProgram.use();
	simpleProgram.setUniformMatrix4f(projectionLoc, *camera.getProjectionMatrix());
	simpleProgram.setUniformMatrix4f("modelview", (*camera.getViewMatrix())*(*pirate.getModel()));
	//vec4 realLightPos = (*camera.getViewMatrix())*lightPos;
	vec4 realLightPos = lightPos;
	simpleProgram.setUniformMatrix3f("normalMatrix", mat3(*camera.getViewMatrix()));
	simpleProgram.setUniform4f("lightAmbient", lightAmbient);
	simpleProgram.setUniform4f("lightDiffuse", lightDiffuse);
	simpleProgram.setUniform4f("lightPosition", realLightPos);
	simpleProgram.setUniform4f("matDiffuse", 1, 1, 1, 1);
	simpleProgram.setUniform4f("matAmbient", 1, 1, 1, 1);
	pirate.render();

	/*mat4 modelView = (*camera.getViewMatrix())*(*sphere1.getModel());
	simpleProgram.setUniformMatrix3f("normalMatrix", mat3(modelView));
	simpleProgram.setUniformMatrix4f("modelview", modelView);
	sphere1.render();

	modelView = (*camera.getViewMatrix())*(*sphere2.getModel());
	simpleProgram.setUniformMatrix3f("normalMatrix", mat3(modelView));
	simpleProgram.setUniformMatrix4f("modelview", modelView);
	sphere2.render();

	modelView = (*camera.getViewMatrix())*(*lightSphere.getModel());
	simpleProgram.setUniformMatrix3f("normalMatrix", mat3(modelView));
	simpleProgram.setUniformMatrix4f("modelview", modelView);
	lightSphere.render();*/
}

void Scene::resize(int w, int h) {
	camera.resize(w, h);
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


	vShader.initFromFile(VERTEX_SHADER, "shaders/lambert.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/lambert.frag");
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