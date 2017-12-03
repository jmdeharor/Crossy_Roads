#include "ShadowedObject.h"
using namespace glm;

mat4 ShadowedObject::getShadowModel() {
	return shadowProjection*model;
}

void ShadowedObject::renderShadow(ShaderProgram & program) {
	if (modified)
		updateModel();
	program.setUniformMatrix4f((uint)UniformLocation::modelLoc, shadowProjection*model);
	mesh->render(program);
}

void ShadowedObject::setPlane(vec4 plane, vec3 direction) {
	float a = plane.x;
	float b = plane.y;
	float c = plane.z;
	float d = plane.w;
	float x = direction.x;
	float y = direction.y;
	float z = direction.z;
	shadowProjection = transpose(mat4(
		b*y+c*z, -b*x, -c*x, -d*x,
		-a*y, a*x+c*z, -c*y, -d*y,
		-a*z, -b*z, a*x+b*y, -d*z,
		0,0,0,a*x + b*y + c*z
	));
}

ShadowedObject::ShadowedObject() {
}


ShadowedObject::~ShadowedObject()
{
}
