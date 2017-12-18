#pragma once
#include "GameObject.h"
#include "ShadowedObject.h"
#include "ImportedMesh.h"
#include "CubeMesh.h"
#include "TexturedObject.h"
#include "ObjectContainer.h"
#include "Jumper.h"
#include "Biome.h"

enum FloorType {
	Road,
	Safe
};

enum BiomeType {
	Ship,
	Sea
};

class FloorRow : public ObjectContainer {
private:
	BiomeType biomeType;
	Biome* biome;
	bool safeZone;
	float rowHeight;
	glm::vec2 pos;
	RowVectors v;

	static FloorParams fp;
	static float offset;
public:
	static void initIds(const Assets& assets);
	static void setParameters(const FloorParams& floorParams);
	static float worldToCol(float x);

	glm::vec2 getPos() const;
	void setPos(glm::vec2 position);
	void initRoad(BiomeType biome, std::vector<glm::uint>& adjacentRow, const std::vector<CellProperties>& map);
	void initSafeZone(BiomeType type, const std::vector<CellProperties>& map);
	void update(int deltaTime);
	void groupDrawableObjects(
		std::vector<std::vector<Object*>>& objects,
		std::vector<std::vector<TexturedObject*>>& texturedObjects,
		const FrustumG& frustum
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

