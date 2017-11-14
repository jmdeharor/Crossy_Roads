#include "Object.h"
#include "glm/gtc/matrix_transform.hpp"
using namespace glm;

void Object::update(int deltaTime)
{
}

void Object::setPos(vec3 pos) {
	this->pos = pos;
}

const mat4 * Object::getModel() const {
	return &model;
}

void Object::updateModel() {
	model = translate(mat4(1), pos);
	model = rotate(model, rot.x, vec3(1, 0, 0));
	model = rotate(model, rot.y, vec3(0, 1, 0));
	model = rotate(model, rot.z, vec3(0, 0, 1));
	model = glm::scale(model, scale);
	model = translate(model, -center);
}

void Object::render() {
	mesh->render();
}

Object::Object() : scale(vec3(1)) {
}

void Object::setMesh(Mesh * mesh) {
	this->mesh = mesh;
}


Object::~Object()
{
}
