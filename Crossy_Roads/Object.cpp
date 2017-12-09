#include "Object.h"
#include "glm/gtc/matrix_transform.hpp"
#include "AABox.h"
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

bool Object::isInsideViewFrustrum(const FrustumG& frustum) {
	const vec3* bbox = mesh->getbb();
	if (modified)
		updateModel();
	vec3 mins = vec3(model*vec4(bbox[0], 1));
	vec3 maxs = vec3(model*vec4(bbox[1], 1));
	vec3 size = maxs - mins;
	AABox box(Vec3(mins), size.x, size.y, size.z);
	return frustum.boxInFrustum(box) != FrustumG::OUTSIDE;
}

bool Object::collidesWith(Object& otherObject) {
	const vec3* bbox1 = mesh->getbb();
	const vec3* bbox2 = otherObject.mesh->getbb();
	if (modified)
		updateModel();
	if (otherObject.modified)
		otherObject.updateModel();
	vec3 mins1 = vec3(model*vec4(bbox1[0], 1));
	vec3 maxs1 = vec3(model*vec4(bbox1[1], 1));
	vec3 mins2 = vec3(otherObject.model*vec4(bbox2[0], 1));
	vec3 maxs2 = vec3(otherObject.model*vec4(bbox2[1], 1));

	return (mins1[0] <= maxs2[0] && maxs1[0] >= mins2[0]) &&
		(mins1[1] <= maxs2[1] && maxs1[1] >= mins2[1]) &&
		(mins1[2] <= maxs2[2] && maxs1[2] >= mins2[2]);
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
	//program.setUniformMatrix4f("model", model);
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