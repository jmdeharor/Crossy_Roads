#pragma once
#include "CubeMesh.h"

class MultiCubeMesh : public CubeMesh {
public:
	void init(glm::uint N);
	MultiCubeMesh();
	~MultiCubeMesh();
};

