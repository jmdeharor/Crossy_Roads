#pragma once
#include <glm\glm.hpp>
typedef int IdMesh;
typedef int IdTex;

#define INVALID -1

class RandomPickMesh {
	glm::uint size;
	IdMesh firstMesh;
	float* probabilities;
public:
	void setMeshes(IdMesh firstMesh, float* probabilities, glm::uint size);
	IdMesh getMesh() const;
	RandomPickMesh();
	~RandomPickMesh();
};

