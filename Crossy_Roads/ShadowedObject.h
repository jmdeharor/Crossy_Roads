#pragma once
#include "Object.h"

class ShadowedObject : public Object {
	glm::mat4 shadowProjection;
public:
	void renderShadow(ShaderProgram& program);
	void setPlane(glm::vec4 plane, glm::vec3 direction);
	ShadowedObject();
	~ShadowedObject();
};

