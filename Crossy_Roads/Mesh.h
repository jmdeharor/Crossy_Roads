#pragma once
#include <vector>
#include <glm\glm.hpp>
#include <GL\glew.h>
#include "RenderizableObject.h"
#include "ShaderProgram.h"

class Mesh : public RenderizableObject {
protected:
	GLuint VAO;
	GLuint VBOvert, VBOind, VBOnorm, VBOcols;
	GLint posLocation, colorLocation, normalLocation;
public:
	void initTest(ShaderProgram& program);
	void render();
	Mesh();
	~Mesh();
};

