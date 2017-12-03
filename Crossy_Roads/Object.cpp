#include "Object.h"
#include "glm/gtc/matrix_transform.hpp"
using namespace glm;

void Object::setPos(vec3 pos) {
	modified = true;
	this->pos = pos;
}

void Object::move(float x, float y, float z) {
	modified = true;
	pos.x += x;
	pos.y += y;
	pos.z += z;
}

void Object::move(vec3 movement) {
	modified = true;
	pos += movement;
}

void Object::rotateX(float angle) {
	modified = true;
	rot.x += angle;
}

void Object::rotateY(float angle) {
	modified = true;
	rot.y += angle;
}

void Object::rotateZ(float angle) {
	modified = true;
	rot.z += angle;
}

void Object::setRotationX(float angle) {
	modified = true;
	rot.x = angle;
}

void Object::setRotationY(float angle) {
	modified = true;
	rot.y = angle;
}

void Object::setRotationZ(float angle) {
	modified = true;
	rot.z;
}

void Object::setScale(vec3 scale) {
	modified = true;
	this->scale = scale;
}

const mat4 * Object::getModel() {
	if (modified)
		updateModel();
	return &model;
}

bool Object::isInsideViewFrustrum(glm::mat4& viewProjection) {
	const vec3* bbox = mesh->getbb();
	if (modified)
		updateModel();
	mat4 modelViewProjection = viewProjection*model;
	vec4 point1 = modelViewProjection*vec4(bbox[0], 1);
	vec4 point2 = modelViewProjection*vec4(bbox[1], 1);
	vec4 point3 = modelViewProjection*vec4(bbox[2], 1);
	vec4 point4 = modelViewProjection*vec4(bbox[3], 1);
	vec4 point5 = modelViewProjection*vec4(bbox[4], 1);
	vec4 point6 = modelViewProjection*vec4(bbox[5], 1);
	vec4 point7 = modelViewProjection*vec4(bbox[6], 1);
	vec4 point8 = modelViewProjection*vec4(bbox[7], 1);

	return ((-point1.w < point1.x && point1.x < point1.w &&
		-point1.w < point1.y && point1.y < point1.w &&
		-point1.w < point1.z && point1.z < point1.w)||

		(-point2.w < point2.x && point2.x < point2.w &&
		 -point2.w < point2.y && point2.y < point2.w &&
		 -point2.w < point2.z && point2.z < point2.w)||

		(-point3.w < point3.x && point3.x < point3.w &&
		 -point3.w < point3.y && point3.y < point3.w &&
		 -point3.w < point3.z && point3.z < point3.w)||

		(-point4.w < point4.x && point4.x < point4.w &&
		 -point4.w < point4.y && point4.y < point4.w &&
		 -point4.w < point4.z && point4.z < point4.w)||

		(-point5.w < point5.x && point5.x < point5.w &&
		 -point5.w < point5.y && point5.y < point5.w &&
		 -point5.w < point5.z && point5.z < point5.w)||

		(-point6.w < point6.x && point6.x < point6.w &&
		 -point6.w < point6.y && point6.y < point6.w &&
		 -point6.w < point6.z && point6.z < point6.w)||

		(-point7.w < point7.x && point7.x < point7.w &&
		 -point7.w < point7.y && point7.y < point7.w &&
		 -point7.w < point7.z && point7.z < point7.w)||

		(-point8.w < point8.x && point8.x < point8.w &&
		 -point8.w < point8.y && point8.y < point8.w &&
		 -point8.w < point8.z && point8.z < point8.w));
}

uint Object::getTriangles() const {
	return 0;//mesh->totalTriangles;
}

void Object::setCenter(glm::vec3 newCenter) {
	modified = true;
	center = newCenter;
}

void Object::updateModel() {
	modified = false;
	model = translate(mat4(1), pos);
	model = glm::scale(model, scale);
	model = rotate(model, rot.x, vec3(1, 0, 0));
	model = rotate(model, rot.y, vec3(0, 1, 0));
	model = rotate(model, rot.z, vec3(0, 0, 1));
	model = translate(model, -center);
}

void Object::render(ShaderProgram & program) {
	if (modified)
		updateModel();
	program.setUniformMatrix4f((uint)UniformLocation::modelLoc, model);
	program.setUniformMatrix3f((uint)UniformLocation::normalMatrixLoc, mat3(model));
	//program.setUniformMatrix4f("model", model);
	//program.setUniformMatrix3f("normalMatrix", mat3(model));
	//mesh->render(program);
}

Object::Object() : 
	pos(vec3(0)), rot(vec3(0)), scale(vec3(1)),
	center(vec3(0)), model(mat4(1)), meshId(INVALID),
	modified(true) {
}

void Object::setMesh(uint meshId, const Mesh* mesh) {
	this->mesh = mesh;
	this->meshId = meshId;
	center = mesh->getbbCenter();
}

void Object::setMesh(const Mesh * mesh) {
	this->mesh = mesh;
	center = mesh->getbbCenter();
}

void Object::setCenterToBaseCenter() {
	modified = true;
	center = vec3(center.x, center.y - mesh->getHeight()/2.f, center.z);
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