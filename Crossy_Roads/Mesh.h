#pragma once
#include <vector>
#include <glm\glm.hpp>
#include <GL\glew.h>
#include "ShaderProgram.h"

class Mesh {
protected:
	unsigned int nVertices;
	float height;
	glm::vec3 center, bbox[2];
	GLuint VAO;
	GLuint VBOvert, VBOnorm, VBOtex;
	GLenum renderMode;
	Mesh();
	virtual void setProgramParams(ShaderProgram& program) const;
public:
	glm::vec3 getbbCenter() const;
	glm::vec3 getbbSize() const;
	float getHeight() const;
	virtual void render(ShaderProgram& program) const;
	void clear();
	virtual ~Mesh();
};

