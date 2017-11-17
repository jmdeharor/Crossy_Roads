#include "Object.h"
#include "glm/gtc/matrix_transform.hpp"
using namespace glm;

void Object::update(int deltaTime)
{
}

void Object::setPos(vec3 pos) {
	this->pos = pos;
}

void Object::move(float x, float y, float z) {
	pos.x += x;
	pos.y += y;
	pos.z += z;
}

void Object::rotateX(float angle) {
	rot.x += angle;
}

void Object::rotateY(float angle) {
	rot.y += angle;
}

void Object::rotateZ(float angle) {
	rot.z += angle;
}

void Object::setScale(vec3 scale) {
	this->scale = scale;
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
	program->setUniformMatrix4f((uint)UniformLocation::modelLoc, model);
	program->setUniformMatrix3f((uint)UniformLocation::normalMatrixLoc, mat3(model));
	mesh->render(*program);
}

Object::Object() : 
	pos(vec3(0)), rot(vec3(0)), scale(vec3(1)),
	center(vec3(0)), model(mat4(1)), mesh(NULL) {
}

void Object::setMesh(const Mesh * mesh) {
	this->mesh = mesh;
	center = mesh->getbbCenter();
}

void Object::setShader(ShaderProgram * program) {
	this->program = program;
}

Object::~Object()
{
}

glm::vec3 Object::getPos() const {
	return pos;
}

float Object::getHeight() const {
	return mesh->getHeight()*scale.y;
}
