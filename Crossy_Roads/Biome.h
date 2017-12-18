#pragma once
#include <vector>
#include <glm\glm.hpp>
#include "Assets.h"
#include "TexturedObject.h"
#include "Jumper.h"
#include <string>

struct CellProperties {
	glm::uint rows, cols;
	float height;
	IdMesh mesh;
};

struct FloorParams {
	glm::vec2 tileSize;
	glm::uint cols;
	glm::uint colOffset;
	float realTileSize;
	glm::vec3 lightDir;
};

struct RowVectors {
	std::vector<Jumper> enemies;
	std::vector<float> speeds;
	std::vector<CellProperties> rowObjects;
	std::vector<ShadowedObject> furniture;
	std::vector<TexturedObject> floorTiles;
	std::vector<ShadowedObject> platforms;
};

class Biome {
protected:
	RowVectors& v;

	static FloorParams fp;
	static const Assets* assets;
	static std::vector<IdMesh> enemyMeshes;
	static std::vector<std::vector<IdTex>> floorTextures;
	static IdMesh planeWood, water;
	static IdMesh crocodile, shark;
public:
	static std::vector<glm::uint> indices;
	static const vector<vector<string>> models;
	static const vector<string> enemyMeshesStrings;
	float rowHeight;
	glm::vec2 pos;
public:
	static void initIds(const Assets& assets);
	static void setParameters(const FloorParams& floorParams);
	virtual void initRoad(std::vector<glm::uint>& adjacentRow, const std::vector<CellProperties>& map) = 0;
	virtual void initSafeZone(const std::vector<CellProperties>& map);
	Biome(RowVectors& vectors);
	virtual ~Biome();
};

