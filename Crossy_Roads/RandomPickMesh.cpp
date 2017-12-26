#include "RandomPickMesh.h"

void RandomPickMesh::setMeshes(IdMesh * meshes, float * probabilities) {
	this->meshes = meshes;
	this->probabilities = probabilities;
}

RandomPickMesh::RandomPickMesh() : meshes(NULL), probabilities(NULL)
{
}


RandomPickMesh::~RandomPickMesh() {
	if (meshes != NULL) {
		delete meshes;
		delete probabilities;
	}
}
