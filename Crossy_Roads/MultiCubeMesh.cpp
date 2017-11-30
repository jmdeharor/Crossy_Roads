#include "MultiCubeMesh.h"
#include <glm\glm.hpp>
#include <vector>
using namespace std;
using namespace glm;

void MultiCubeMesh::init(uint N) {
	// 6 caras de 4 vertices cada una
	vector<float> vert;
	vector<float> norm;
	vector<float> tex;
	// 6 caras de 2 triangulos cada una
	vector<uint> ind;

	vert.reserve(N * 6 * 4 * 3);
	norm.reserve(N * 6 * 4 * 3);
	tex.reserve(N * 6 * 4 * 2);
	ind.reserve(N * 6 * 2 * 3);

	static const float vertices[] = {
		-1.0f, -1.0f, -1.f,
		1.0f, -1.0f, -1.f,
		-1.0f,  1.0f, -1.f,
		1.0f,  1.0f, -1.f,

		-1.0f, -1.0f, 1.f,
		1.0f, -1.0f, 1.f,
		-1.0f,  1.0f, 1.f,
		1.0f,  1.0f, 1.f,

		-1.0f, -1.f, -1.0f,
		1.0f, -1.f, -1.0f,
		-1.0f, -1.f,  1.0f,
		1.0f, -1.f,  1.0f,

		-1.0f, 1.f, -1.0f,
		1.0f, 1.f, -1.0f,
		-1.0f, 1.f,  1.0f,
		1.0f, 1.f,  1.0f,

		-1.f, -1.0f, -1.0f,
		-1.f,  1.0f, -1.0f,
		-1.f, -1.0f,  1.0f,
		-1.f,  1.0f,  1.0f,

		1.f, -1.0f, -1.0f,
		1.f,  1.0f, -1.0f,
		1.f, -1.0f,  1.0f,
		1.f,  1.0f,  1.0f,
	};
	static const float normals[] = {
		0,0,-1,
		0,0,-1,
		0,0,-1,
		0,0,-1,

		0,0,1,
		0,0,1,
		0,0,1,
		0,0,1,

		0,-1,0,
		0,-1,0,
		0,-1,0,
		0,-1,0,

		0,1,0,
		0,1,0,
		0,1,0,
		0,1,0,

		-1,0,0,
		-1,0,0,
		-1,0,0,
		-1,0,0,

		1,0,0,
		1,0,0,
		1,0,0,
		1,0,0
	};

	//0-0.5 0-0.33 cara y
	//0.5-1 0-0.33 cara -y
	//0-0.5 0.33-0.66 cara -z
	//0.5-1 0-33-0.66 cara z
	//0-0.5 0.66-1 cara -x
	//0.5-1 0.66-1 cara x
	static const float sY = 1.f / 3;
	static const float texCoords[] = {
		0,sY,
		0.5,sY,
		0,2 * sY,
		0.5,2 * sY,

		0.5,sY,
		1,sY,
		0.5,2 * sY,
		1,2 * sY,

		0.5,0,
		1,0,
		0.5,sY,
		1,sY,

		0,0,
		0.5,0,
		0,sY,
		0.5,sY,

		0,2 * sY,
		0.5,2 * sY,
		0,1,
		0.5,1,

		0.5,2 * sY,
		1,2 * sY,
		0.5,1,
		1,1
	};

	static const GLuint indices[] = {
		0,1,2,
		1,2,3,

		4,5,6,
		5,6,7,

		8,9,10,
		9,10,11,

		12,13,14,
		13,14,15,

		16,17,18,
		17,18,19,

		20,21,22,
		21,22,23
	};

	for (uint i = 0; i < N; ++i) {
		for (uint j = 0; j < 6*4; ++j) {
			vert.push_back(vertices[j*3]+i*2);
			vert.push_back(vertices[j*3 + 1]);
			vert.push_back(vertices[j*3 + 2]);

			norm.push_back(normals[j*3]);
			norm.push_back(normals[j * 3 +1]);
			norm.push_back(normals[j * 3+1]);

			tex.push_back(texCoords[j * 2]);
			tex.push_back(texCoords[j * 2 + 1]);
		}
		for (uint j = 0; j < 6 * 2 * 3; ++j) {
			ind.push_back(indices[j]+ i*6*4);
		}
	}

	
	bbox[0] = vec3(-1, -1, -1);
	bbox[1] = vec3(2*N, 1, 1);
	height = bbox[1].y-bbox[0].y;
	center = (bbox[1]+bbox[0])/2.f;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBOvert);
	glBindBuffer(GL_ARRAY_BUFFER, VBOvert);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vert.size(), &vert[0], GL_STATIC_DRAW);

	glGenBuffers(1, &VBOnorm);
	glBindBuffer(GL_ARRAY_BUFFER, VBOnorm);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*norm.size(), &norm[0], GL_STATIC_DRAW);

	glGenBuffers(1, &VBOtex);
	glBindBuffer(GL_ARRAY_BUFFER, VBOtex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*tex.size(), &tex[0], GL_STATIC_DRAW);

	glGenBuffers(1, &VBOind);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOind);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float)*ind.size(), &ind[0], GL_STATIC_DRAW);

	nVertices = vert.size();
	totalTriangles = ind.size()/3;
}

MultiCubeMesh::MultiCubeMesh()
{
}


MultiCubeMesh::~MultiCubeMesh()
{
}
