#pragma once
#include "MeshConfigConstructor.h"
#include "RandomPicker.h"

class RandomMeshConfig : public MeshConfigConstructor {
	RandomPicker randomPicker;
public:
	IdMesh firstMesh;
	float* heights;
	bool* empty, *collisionMap;
	glm::uint rows, cols;
	bool canJump;
	void setProbabilities(const float* probabilities, glm::uint size);
	void store(std::ofstream& stream) override;
	void load(std::ifstream& stream) override;
	MeshConfig getMeshConfig() const override;
	glm::uint getRows() const override;
	glm::uint getCols() const override;
	~RandomMeshConfig();
};

