#pragma once
#include "CubeMesh.h"

class MultiCubeMesh : public CubeMesh {
public:
	void init(uint N);
	MultiCubeMesh();
	~MultiCubeMesh();
};

