#pragma once
#include "Object.h"

class ShadowedObject : public Object {
	glm::mat4 shadowProjection;
	ShaderProgram* shadowProgram;
public:
	void renderShadow();
	void setShadowShader(ShaderProgram* program);
	void setPlane(glm::vec4 plane, glm::vec3 direction);
	ShadowedObject();
	~ShadowedObject();
};

