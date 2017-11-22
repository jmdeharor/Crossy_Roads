#include "QuadMesh.h"
#include <iostream>
using namespace glm;
using namespace std;

void QuadMesh::init() {
	renderMode = GL_TRIANGLE_STRIP;

	static const float vertices[] = {
		-1.0f, -1.0f, 0,
		 1.0f, -1.0f, 0,
		-1.0f,  1.0f, 0,
		 1.0f,  1.0f, 0,
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

	nVertices = 4;
}

QuadMesh::QuadMesh()
{
}

QuadMesh::~QuadMesh() {
	clear();
}
