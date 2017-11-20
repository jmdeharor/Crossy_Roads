#include "Renderer.h"
#include "Game.h"
#include <iostream>
using namespace std;
using namespace glm;

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::init() {
	GameObject::init();
}

void Renderer::setLightParameters(glm::vec3 lightDir, glm::vec4 lightAmbient, glm::vec4 lightDiffuse) {
	this->lightDir = lightDir;
	this->lightAmbient = lightAmbient;
	this->lightDiffuse = lightDiffuse;
}

void Renderer::setCamera(Camera * camera) {
	this->camera = camera;
}

void Renderer::addGroup(Object * objectArray, uint size) {
	ShadowedObject* cast = dynamic_cast<ShadowedObject*>(objectArray);
	objects.reserve(objects.size() + size);
	if (cast) {
		shadowedObjects.reserve(shadowedObjects.size() + size);
		for (uint i = 0; i < size; ++i) {
			objects.push_back(&objectArray[i]);
			if (cast) {
				shadowedObjects.push_back((ShadowedObject*)&objectArray[i]);
			}
		}
	}
	
}

void Renderer::addSimpleGroup(Object * objects, uint size) {
	simpleObjects.reserve(simpleObjects.size() + size);
	for (uint i = 0; i < size; ++i) {
		simpleObjects.push_back(&objects[i]);
	}
}

inline void applyShader(ShaderProgram& program, std::vector<Object*>& objects) {
	for (Object* object : objects) {
		object->render(program);
	}
}

inline void applyShadowShader(ShaderProgram& program, std::vector<ShadowedObject*>& objects) {
	for (ShadowedObject* object : objects) {
		object->renderShadow(program);
	}
}

void Renderer::render() {
	glEnable(GL_TEXTURE_2D);
	texProgram.use();
	texProgram.setUniformMatrix4f(projectionLoc, *camera->getProjectionMatrix());
	texProgram.setUniformMatrix4f(viewLoc, *camera->getViewMatrix());
	texProgram.setUniform4f("color", vec4(1));
	applyShader(texProgram, simpleObjects);

	lambertProgram.use();
	lambertProgram.setUniformMatrix4f(projectionLoc, *camera->getProjectionMatrix());
	lambertProgram.setUniformMatrix4f(viewLoc, *camera->getViewMatrix());
	vec3 lightD = mat3(*camera->getViewMatrix())*lightDir;
	lambertProgram.setUniform4f("lightAmbient", lightAmbient);
	lambertProgram.setUniform4f("lightDiffuse", lightDiffuse);
	lambertProgram.setUniform3f("lightDirection", lightD.x, lightD.y, lightD.z);
	lambertProgram.setUniform4f("matDiffuse", 1, 1, 1, 1);
	lambertProgram.setUniform4f("matAmbient", 1, 1, 1, 1);
	applyShader(lambertProgram, objects);

	glDisable(GL_TEXTURE_2D);

	shadowProgram.use();
	shadowProgram.setUniformMatrix4f(projectionLoc, *camera->getProjectionMatrix());
	shadowProgram.setUniformMatrix4f(viewLoc, *camera->getViewMatrix());
	glEnable(GL_POLYGON_OFFSET_FILL);
	glEnable(GL_BLEND);
	glEnable(GL_STENCIL_TEST);
	glPolygonOffset(-1, -1);

	applyShadowShader(shadowProgram, shadowedObjects);

	glDisable(GL_STENCIL_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_POLYGON_OFFSET_FILL);
}

void Renderer::update(int deltaTime) {
	if (Game::instance().getKey('m')) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else if (Game::instance().getKey('k')) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
}

void Renderer::initShaders() {
	/*compileShader(texProgram, "texture");
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
	drawImageProgram.bindFragmentOutput("outColor");*/
}

void Renderer::firstInit() {
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glStencilFunc(GL_EQUAL, 0, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
	initShaders();
}
