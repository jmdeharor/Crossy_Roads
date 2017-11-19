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

void Renderer::addGroup(Object * objectArray, uint size, uint stride) {
	ObjectVector objectVector;
	objectVector.vector = (byte*)objectArray;
	objectVector.size = size;
	objectVector.stride = stride;
	objects.push_back(objectVector);

	ShadowedObject* cast = dynamic_cast<ShadowedObject*>(objectArray);
	if (cast) {
		shadowedObjects.push_back(objectVector);
	}
}

void Renderer::addSimpleGroup(Object * objects, uint size, uint stride) {
	ObjectVector objectVector;
	objectVector.vector = (byte*)objects;
	objectVector.size = size;
	objectVector.stride = stride;
	simpleObjects.push_back(objectVector);
}

inline void applyShader(ShaderProgram& program, std::vector<ObjectVector>& objects) {
	for (uint i = 0; i < objects.size(); ++i) {
		byte* objectGroup = objects[i].vector;
		uint size = objects[i].size;
		uint stride = objects[i].stride;
		for (uint j = 0; j < size; ++j) {
			Object* object = (Object*)&objectGroup[j*stride];
			object->render(program);
		}
	}
}

inline void applyShadowShader(ShaderProgram& program, std::vector<ObjectVector>& objects) {
	for (uint i = 0; i < objects.size(); ++i) {
		byte* objectGroup = objects[i].vector;
		uint size = objects[i].size;
		uint stride = objects[i].stride;
		for (uint j = 0; j < size; ++j) {
			ShadowedObject* object = (ShadowedObject*)&objectGroup[j*stride];
			object->renderShadow(program);
		}
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
	vShader.free();
	fShader.free();
	for (uint i = 0; i < sizeof(uniformOrder) / sizeof(string); ++i) {
		program.addUniform(uniformOrder[i]);
	}
}

void Renderer::initShaders() {
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

void Renderer::firstInit() {
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glStencilFunc(GL_EQUAL, 0, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
	initShaders();
}
