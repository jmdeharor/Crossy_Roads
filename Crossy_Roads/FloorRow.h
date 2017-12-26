#pragma once
#include "GameObject.h"
#include "ShadowedObject.h"
#include "ImportedMesh.h"
#include "CubeMesh.h"
#include "TexturedObject.h"
#include "ObjectContainer.h"
#include "Jumper.h"
#include "FloorResources.h"

enum FloorType {
	Road,
	Safe
};

enum BiomeType {
	Ship,
	Sea
};

struct CellProperties {
	glm::uint rows, cols;
	float height, verticalOffset;
	bool collision, occupied, empty;
	IdMesh mesh;
	CellProperties() : 
		mesh(INVALID), 
		collision(false), occupied(false),  empty(false),
		height(0), verticalOffset(0) {

	}
};

struct FloorParams {
	glm::vec2 tileSize;
	glm::uint cols;
	glm::uint colOffset;
	float realTileSize;
	glm::vec3 lightDir;
};

class FloorRow : public ObjectContainer {
private:
	BiomeType biomeType;
	bool safeZone;
	float rowHeight;
	glm::uint frameCounter, frameLimit;

	std::vector<Jumper> enemies;
	std::vector<float> speeds;
	std::vector<CellProperties> map;
	std::vector<ShadowedObject> furniture;
	std::vector<TexturedObject> floorTiles;
	std::vector<ShadowedObject> platforms;

	static FloorParams fp;
	static FloorResources res;
	static float offset;
private:
	void initShipRoad(std::vector<glm::uint>& adjacentRow);
	void initShipSafeZone();
	void initSeaRoad(std::vector<glm::uint>& adjacentRow);
public:
	glm::vec2 pos;
public:
	static void initResources(const Assets& assets);
	static void setParameters(const FloorParams& floorParams);
	static glm::uint worldToCol(float x);

	void initRoad(BiomeType biome, std::vector<glm::uint>& adjacentRow, const std::vector<CellProperties>& map);
	void initSafeZone(BiomeType type, const std::vector<CellProperties>& map);
	void update(int deltaTime);
	void groupDrawableObjects(
		const FrustumG& frustum, RenderVectors& renderVectors
	) override;
	pair<glm::vec3, float> getHeight(glm::uint col);
	std::vector<Jumper>* getEnemies();
	std::vector<CellProperties>* getRowObjects();
	std::vector<ShadowedObject>* getPlatforms();
	bool isSafeZone() const;
	BiomeType getBiome() const;
	FloorRow();
	~FloorRow();
};

