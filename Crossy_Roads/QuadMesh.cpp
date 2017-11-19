#include "QuadMesh.h"
#include <iostream>
using namespace glm;
using namespace std;

void QuadMesh::init() {
	static const float vertices[] = {
		-1.0f, 0, -1.0f,
		 1.0f, 0, -1.0f,
		-1.0f, 0,  1.0f,
		 1.0f, 0,  1.0f,
	};
	static const float normals[] = {
		0,1,0,
		0,1,0,
		0,1,0,
		0,1,0
	};
	static const float texCoords[] = {
		0,0,
		1,0,
		0,1,
		1,1
	};
	bbox[0] = vec3(-1);
	bbox[1] = vec3(1);
	height = 0;
	center = vec3(0);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBOvert);
	glBindBuffer(GL_ARRAY_BUFFER, VBOvert);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBOnorm);
	glBindBuffer(GL_ARRAY_BUFFER, VBOnorm);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

	glGenBuffers(1, &VBOtex);
	glBindBuffer(GL_ARRAY_BUFFER, VBOtex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);

	nTriangles = 4;
}

void QuadMesh::render(ShaderProgram& program) const {
	texture.use();
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(positionLoc);
	glEnableVertexAttribArray(normalLoc);
	glEnableVertexAttribArray(texCoordLoc);

	glBindBuffer(GL_ARRAY_BUFFER, VBOvert);
	program.bindVertexAttribute(positionLoc, 3, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBOnorm);
	program.bindVertexAttribute(normalLoc, 3, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBOtex);
	program.bindVertexAttribute(texCoordLoc, 2, 0, 0);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, nTriangles);
}

void QuadMesh::setQuadTexture(const string & fileName) {
	texture.loadFromFile(fileName, TEXTURE_PIXEL_FORMAT_RGB);
}

QuadMesh::QuadMesh()
{
}

QuadMesh::~QuadMesh() {
	clear();
}
