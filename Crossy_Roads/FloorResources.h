#pragma once
#include "Assets.h"
#include <vector>

class FloorResources {
public:
	const Assets* assets;
	const std::vector<IdMesh>* groups;
	std::vector<std::vector<IdTex>> floorTextures;
	std::vector<glm::uint> indices;
	IdTex planeWood, water, islandRoad, islandSafe;
	std::pair<IdTex, glm::uint> waterAnimation;
	std::pair<IdMesh, glm::uint> sharkAnimation;
	const vector<vector<string>> models;
	float rangeLeft, rangeRight;

	void init(const Assets& assets);
	FloorResources();
	~FloorResources();
};

