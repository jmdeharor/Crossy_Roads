#pragma once
#include <glm\glm.hpp>
typedef int IdMesh;
typedef int IdTex;

#define INVALID -1

class RandomPickMesh {
	glm::uint size;
	IdMesh* meshes;
	float* probabilities;
public:
	void setMeshes(IdMesh* meshes, float* probabilities, glm::uint size);
	IdMesh getMesh() const;
	RandomPickMesh();
	~RandomPickMesh();
};

