#pragma once
#include "Assets.h"
#include <vector>

class FloorResources {
public:
	const Assets* assets;
	std::vector<IdMesh> enemyMeshes;
	std::vector<std::vector<IdTex>> floorTextures;
	std::vector<glm::uint> indices;
	IdMesh planeWood, water;
	IdMesh crocodile, shark;
	const vector<vector<string>> models;
	const vector<string> enemyMeshesStrings;

	void init(const Assets& assets);
	FloorResources();
	~FloorResources();
};

