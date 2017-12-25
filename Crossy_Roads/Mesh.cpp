#include "Mesh.h"
#include <assimp\Importer.hpp>
#include <iostream>
#include <assimp/postprocess.h>
using namespace std;
using namespace glm;

const glm::vec3 * Mesh::getbb() const {
	return bbox;
}

glm::vec3 Mesh::getbbCenter() const {
	return center;
}

glm::vec3 Mesh::getbbSize() const {
	return bbox[1]-bbox[0];
}

float Mesh::getHeight() const {
	return height;
}

void Mesh::setProgramParams(ShaderProgram & program) const {
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
}

#include "Scene.h"

void Mesh::render() const {
	//setProgramParams(program);
	Scene::sceneTriangles += totalTriangles;
	Scene::sceneDrawCalls += 1;
	glDrawArrays(renderMode, 0, nVertices);
}

void Mesh::clear() {
	glDeleteBuffers(1, &VBOvert);
	glDeleteBuffers(1, &VBOnorm);
	glDeleteBuffers(1, &VBOtex);
	glDeleteVertexArrays(1, &VAO);
}

Mesh::Mesh() : renderMode(GL_TRIANGLES), totalTriangles(0) {
}

void Mesh::generateAllbbPoints() {
	bbox[2] = vec3(bbox[0].x, bbox[0].y, bbox[1].z);
	bbox[3] = vec3(bbox[0].x, bbox[1].y, bbox[0].z);
	bbox[4] = vec3(bbox[0].x, bbox[1].y, bbox[1].z);
	bbox[5] = vec3(bbox[1].x, bbox[0].y, bbox[0].z);
	bbox[6] = vec3(bbox[1].x, bbox[0].y, bbox[1].z);
	bbox[7] = vec3(bbox[1].x, bbox[1].y, bbox[0].z);
}

Mesh::~Mesh() {
	clear();
}