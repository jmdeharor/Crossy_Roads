#pragma once
#include "ShadowedObject.h"
#include "ImportedMesh.h"
#include "CubeMesh.h"
#include "TexturedObject.h"
#include "ObjectContainer.h"
#include "Jumper.h"
#include "FloorResources.h"
#include "AnimTexObject.h"
#include "AnimMeshObject.h"
#include "BehaviourObject.h"

enum FloorType {
	Road,
	Safe
};

struct CellProperties {
	glm::uint rows, cols;
	float height, verticalOffset;
	bool collision, occupied, empty;
	bool canJump;
	IdMesh mesh;
	CellProperties() : 
		mesh(INVALID), 
		canJump(false), collision(false), occupied(false),  empty(false),
		height(0), verticalOffset(0) {
	}
};

struct FloorParams {
	float lowerLimit; //Right
	float upperLimit; //Left
	glm::vec2 tileSize;
	glm::uint cols;
	glm::uint colOffset;
	float realTileSize;
	glm::vec3 lightDir;
};

class FloorRow : public ObjectContainer {
private:
	BiomeType biome;
	bool safeZone;
	float rowHeight;
	glm::uint frameCounter, frameLimit;
	std::vector<Jumper> enemies;
	std::vector<float> speeds;
	std::vector<CellProperties> map;
	std::vector<BehaviourObject*> furniture;
	std::vector<TexturedObject> floorTiles;
	std::vector<AnimTexObject> animatedFloorTiles;
	std::vector<AnimMeshObject> platforms;

	static FloorParams fp;
	static FloorResources res;
	static float offset;
private:
	void initShipRoad(std::vector<glm::uint>& adjacentRow);
	void initShipSafeZone(const FloorRow& prevRow);
	void initSeaRoad(const FloorRow& prevRow);
	void initIslandRoad();
	void initIslandSafeZone(const FloorRow& prevRow);
public:
	glm::vec2 pos;
public:
	static void initResources(const Assets& assets, const Player* player);
	static void setParameters(const FloorParams& floorParams);
	static glm::uint worldToCol(float x);
	
	void firstInit();
	void initAttributes(BiomeType biome, bool safeZone, float rowHeight);
	void initRoad(BiomeType biome, std::vector<glm::uint>& adjacentRow, const std::vector<CellProperties>& map, const FloorRow& prevRow);
	void initSafeZone(BiomeType biome, const std::vector<CellProperties>& map, const FloorRow& prevRow);
	void update(int deltaTime);
	void groupDrawableObjects(
		const FrustumG& frustum, RenderVectors& renderVectors
	) override;
	pair<glm::vec3, float> getNextPos(glm::uint col);
	float getRowHeight() const;
	std::vector<Jumper>* getEnemies();
	std::vector<CellProperties>* getRowObjects();
	std::vector<AnimMeshObject>* getPlatforms();
	bool isSafeZone() const;
	BiomeType getBiome() const;
	FloorRow();
	~FloorRow();
};

