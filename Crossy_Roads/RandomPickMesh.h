#pragma once
#include "Assets.h"

class RandomPickMesh {
	IdMesh* meshes;
	float* probabilities;
public:
	void setMeshes(IdMesh* meshes, float* probabilities);
	RandomPickMesh();
	~RandomPickMesh();
};

