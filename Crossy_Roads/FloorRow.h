#pragma once
#include "GameObject.h"
#include "ShadowedObject.h"
#include "ImportedMesh.h"
#include "CubeMesh.h"
#include "TexturedObject.h"
#include "ObjectContainer.h"
#include "Jumper.h"

struct CellProperties {
	glm::uint rows, cols;
	float height;
	IdMesh mesh;
};

enum FloorType {
	Road,
	Safe
};

enum Biome {
	Ship,
	Sea
};

class FloorRow : public ObjectContainer {
private:
	Biome biome;
	bool safeZone;
	float rowHeight;
	std::vector<CellProperties> rowObjects;
	std::vector<ShadowedObject> furniture;
	std::vector<TexturedObject> floorTiles;
	std::vector<Jumper> enemies;
	std::vector<ShadowedObject> platforms;
	std::vector<float> speeds;
	glm::vec2 pos;

	const static Assets* assets;
	static glm::vec2 tileSize;
	static glm::uint cols;
	static glm::vec3 lightDir;
	static std::vector<IdMesh> enemyMeshes;
	const static Mesh* cubeMesh;
	static std::vector<vector<IdTex>> floorTextures;
	static IdMesh planeWood, water;
	static IdMesh crocodile;
	static float offset;
	static float realTileSize;
public:
	static void initIds(const Assets& assets);
	void initSafeZone(std::vector<CellProperties>& map);
	static void setParameters(glm::vec2 tileSize, glm::uint cols, glm::vec3 lightDir, glm::uint colOffset);
	glm::vec2 getPos() const;
	void setPos(glm::vec2 position);
	void initRoad(Biome biome, std::vector<glm::uint>* adjacentRow = NULL);
	void initShipRoad(std::vector<glm::uint>& adjacentRow);
	void initSea();
	void update(int deltaTime);
	void groupDrawableObjects(
		std::vector<std::vector<Object*>>& objects,
		std::vector<std::vector<TexturedObject*>>& texturedObjects,
		const FrustumG& frustum
	) override;
	float getHeight(glm::uint col) const;
	std::vector<Jumper>* getEnemies();
	std::vector<CellProperties>* getRowObjects();
	bool isSafeZone() const;
	Biome getBiome() const;
	FloorRow();
	~FloorRow();
};

