#pragma once
#include "MeshConfigConstructor.h"

class BasicMeshConfig : public MeshConfigConstructor {
public:
	MeshConfig meshConfig;
	MeshConfig getMeshConfig() const override;
	void store(std::ofstream& stream) override;
	void load(std::ifstream& stream) override;
	glm::uint getRows() const override;
	glm::uint getCols() const override;
	~BasicMeshConfig();
};

