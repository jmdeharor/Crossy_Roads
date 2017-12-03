#pragma once
#include <vector>
#include <glm\glm.hpp>
#include <GL\glew.h>
#include "ShaderProgram.h"
#include <map>

class Mesh {
protected:
	unsigned int nVertices;
	float height;
	glm::vec3 center, bbox[8];
	GLuint VAO;
	GLuint VBOvert, VBOnorm, VBOtex;
	GLenum renderMode;
protected:
	void generateAllbbPoints();
	Mesh();
public:
	glm::uint totalTriangles;
	const glm::vec3* getbb() const;
	glm::vec3 getbbCenter() const;
	glm::vec3 getbbSize() const;
	float getHeight() const;
	virtual void setProgramParams(ShaderProgram& program) const;
	virtual void render(ShaderProgram& program) const;
	void clear();
	virtual ~Mesh();
};

