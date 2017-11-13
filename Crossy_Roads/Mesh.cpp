#include "Mesh.h"
using namespace std;
using namespace glm;

void Mesh::initTest(ShaderProgram& program) {
	vector<float> vertices = {
		0,0,0,
		0,1,0,
		1,0,0,
		1,1,0
	};
	vector<float> colors = {
		1,1,1,
		1,1,1,
		1,1,1,
		1,1,1
	};
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBOvert);
	glBindBuffer(GL_ARRAY_BUFFER, VBOvert);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 3, 0, 0);
	glGenBuffers(1, &VBOcols);
	glBindBuffer(GL_ARRAY_BUFFER, VBOcols);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), &colors[0], GL_STATIC_DRAW);
	colorLocation = program.bindVertexAttribute("color", 3, 0, 0);
}

void Mesh::render() {
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(colorLocation);
	glDrawArrays(GL_TRIANGLES, 0, 4);
}

Mesh::Mesh()
{
}


Mesh::~Mesh()
{
}
