#include <iostream>
#include <vector>
#include "Billboard.h"


Billboard *Billboard::createBillboard(const glm::vec2 &quadSize, ShaderProgram &program, const string &textureFile, BillboardType billboardType)
{
	Billboard *billboard = new Billboard(quadSize, program, textureFile, billboardType);

	return billboard;
}

Billboard::Billboard(const glm::vec2 &quadSize, ShaderProgram &program, const string &textureFile, BillboardType billboardType) {
	type = billboardType;
	size = quadSize;
	if (!texture.loadFromFile(textureFile.c_str(), TEXTURE_PIXEL_FORMAT_RGBA))
		cout << "Could not load billboard texture!!!" << endl;
	texture.setMagFilter(GL_NEAREST);
	prepareArrays(program);
}

Billboard::~Billboard()
{
}


void Billboard::render(const glm::vec3 &position, const glm::vec3 &eye)
{
	updateArrays(position, eye);

	glEnable(GL_TEXTURE_2D);

	texture.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(positionLoc);
	glEnableVertexAttribArray(normalLoc);
	glEnableVertexAttribArray(texCoordLoc);
	glDrawArrays(GL_QUADS, 0, 4);

	//glDisable(GL_TEXTURE_2D);
}

void Billboard::free()
{
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}


void Billboard::prepareArrays(ShaderProgram &program)
{
	vector<float> vertices;

	// Floor
	vertices.push_back(-size.x / 2.f); vertices.push_back(0.f); vertices.push_back(0.f);
	vertices.push_back(0.f); vertices.push_back(0.f); vertices.push_back(1.f);
	vertices.push_back(0.f); vertices.push_back(1.f);

	vertices.push_back(size.x / 2.f); vertices.push_back(0.f); vertices.push_back(0.f);
	vertices.push_back(0.f); vertices.push_back(0.f); vertices.push_back(1.f);
	vertices.push_back(1.f); vertices.push_back(1.f);

	vertices.push_back(size.x / 2.f); vertices.push_back(size.y); vertices.push_back(0.f);
	vertices.push_back(0.f); vertices.push_back(0.f); vertices.push_back(1.f);
	vertices.push_back(1.f); vertices.push_back(0.f);

	vertices.push_back(-size.x / 2.f); vertices.push_back(size.y); vertices.push_back(0.f);
	vertices.push_back(0.f); vertices.push_back(0.f); vertices.push_back(1.f);
	vertices.push_back(0.f); vertices.push_back(0.f);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 32 * sizeof(float), &vertices[0], GL_DYNAMIC_DRAW);
	program.bindVertexAttribute(positionLoc, 3, 8 * sizeof(float), 0);
	program.bindVertexAttribute(normalLoc, 3, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	program.bindVertexAttribute(texCoordLoc, 2, 8 * sizeof(float), (void *)(6 * sizeof(float)));
}

void Billboard::updateArrays(const glm::vec3 &position, const glm::vec3 &eye)
{
	vector<float> vertices;

	switch (type)
	{
	case BILLBOARD_Y_AXIS:
		prepareBillboardYAxis(position, eye, vertices);
		break;
	default:
		prepareBillboardCenter(position, eye, vertices);
		break;
	}
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 32 * sizeof(float), &vertices[0]);
}

void Billboard::prepareBillboardYAxis(const glm::vec3 &position, const glm::vec3 &eye, vector<float> &vertices)
{
	glm::vec3 xVector, P;

	xVector = glm::normalize(glm::vec3(eye.z - position.z, 0.f, -(eye.x - position.x)));

	P = position - size.x / 2.f * xVector - glm::vec3(0.f, size.y / 2.f, 0.f);
	vertices.push_back(P.x); vertices.push_back(P.y); vertices.push_back(P.z);
	vertices.push_back(0.f); vertices.push_back(0.f); vertices.push_back(1.f);
	vertices.push_back(0.f); vertices.push_back(1.f);

	P = position + size.x / 2.f * xVector - glm::vec3(0.f, size.y / 2.f, 0.f);
	vertices.push_back(P.x); vertices.push_back(P.y); vertices.push_back(P.z);
	vertices.push_back(0.f); vertices.push_back(0.f); vertices.push_back(1.f);
	vertices.push_back(1.f); vertices.push_back(1.f);

	P = position + size.x / 2.f * xVector + glm::vec3(0.f, size.y / 2.f, 0.f);
	vertices.push_back(P.x); vertices.push_back(P.y); vertices.push_back(P.z);
	vertices.push_back(0.f); vertices.push_back(0.f); vertices.push_back(1.f);
	vertices.push_back(1.f); vertices.push_back(0.f);

	P = position - size.x / 2.f * xVector + glm::vec3(0.f, size.y / 2.f, 0.f);
	vertices.push_back(P.x); vertices.push_back(P.y); vertices.push_back(P.z);
	vertices.push_back(0.f); vertices.push_back(0.f); vertices.push_back(1.f);
	vertices.push_back(0.f); vertices.push_back(0.f);
}

void Billboard::prepareBillboardCenter(const glm::vec3 &position, const glm::vec3 &eye, vector<float> &vertices)
{
	glm::vec3 xVector, yVector, P;

	xVector = glm::normalize(glm::cross(glm::vec3(0.f, 1.f, 0.f), eye - position));
	yVector = glm::normalize(glm::cross(eye - position, xVector));

	P = position - size.x / 2.f * xVector - size.y / 2.f * yVector;
	vertices.push_back(P.x); vertices.push_back(P.y); vertices.push_back(P.z);
	vertices.push_back(0.f); vertices.push_back(0.f); vertices.push_back(1.f);
	vertices.push_back(0.f); vertices.push_back(1.f);

	P = position + size.x / 2.f * xVector - size.y / 2.f * yVector;
	vertices.push_back(P.x); vertices.push_back(P.y); vertices.push_back(P.z);
	vertices.push_back(0.f); vertices.push_back(0.f); vertices.push_back(1.f);
	vertices.push_back(1.f); vertices.push_back(1.f);

	P = position + size.x / 2.f * xVector + size.y / 2.f * yVector;
	vertices.push_back(P.x); vertices.push_back(P.y); vertices.push_back(P.z);
	vertices.push_back(0.f); vertices.push_back(0.f); vertices.push_back(1.f);
	vertices.push_back(1.f); vertices.push_back(0.f);

	P = position - size.x / 2.f * xVector + size.y / 2.f * yVector;
	vertices.push_back(P.x); vertices.push_back(P.y); vertices.push_back(P.z);
	vertices.push_back(0.f); vertices.push_back(0.f); vertices.push_back(1.f);
	vertices.push_back(0.f); vertices.push_back(0.f);
}

void Billboard::setType(BillboardType billboardType)
{
	type = billboardType;
}


