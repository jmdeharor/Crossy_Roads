#pragma once
#include "Assets.h"
#include <vector>

class FloorResources {
public:
	const Assets* assets;
	const std::vector<IdMesh>* groups;
	std::vector<std::vector<IdTex>> floorTextures;
	std::vector<glm::uint> indices;
	IdTex planeWood, water;
	IdMesh crocodile, shark;
	const std::vector<IdTex>* waterAnimation;
	const vector<vector<string>> models;

	void init(const Assets& assets);
	FloorResources();
	~FloorResources();
};

