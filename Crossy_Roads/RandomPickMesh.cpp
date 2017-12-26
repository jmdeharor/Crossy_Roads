#include "RandomPickMesh.h"
using namespace glm;

void RandomPickMesh::setMeshes(IdMesh * meshes, float * probabilities, uint size) {
	this->meshes = meshes;
	this->probabilities = probabilities;
}

IdMesh RandomPickMesh::getMesh() const {
	float pick = rand() / (float)(RAND_MAX+1);
	float prob = 0;
	for (uint i = 0; i < size; ++i) {
		if (pick >= prob && pick < prob + probabilities[i]) {
			return meshes[i];
		}
		prob += probabilities[i];
	}
	return INVALID;
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
