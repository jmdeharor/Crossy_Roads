#pragma once
#include "QuadMesh.h"
#include <vector>
#include "ShadowedObject.h"
#include "ImportedMesh.h"
#include "FloorRow.h"
#include "Assets.h"
#include "ObjectContainer.h"
#include "FrustumG.h"

struct MeshConfig {
	glm::uint rows, cols;
	float height;
	IdMesh mesh;
};

class Floor : public GameObject, public ObjectContainer {
	glm::uint rows, cols;
	glm::uint lastRow;
	IdMesh deckMesh[4];
	float firstPos;
	glm::uint colOffset;
	glm::uint rowOffset;
	glm::uint counter;
	glm::uint length;
	std::vector<MeshConfig> furniture;
	std::vector<FloorRow> floorRows;
	std::vector<std::vector<CellProperties>> safeZoneMap;
	std::vector<glm::uint> textureIndex;
	FloorType type;
	glm::vec2 tileSize;
private:
	void updateFloorRow(FloorRow& floorRow);
public:
	void addLevel();
	void firstInit() override;
	void init(glm::vec3 lightDir, const Assets& assets);
	void update(int deltaTime);
	void groupDrawableObjects(
		std::vector<std::vector<Object*>>& objects,
		std::vector<std::vector<TexturedObject*>>& texturedObjects,
		const FrustumG& frustum
	) override;
	FloorRow* getFloorRow(glm::uint index);
	glm::uint getRows() const;
	glm::uint getCols() const;
	glm::uint getColOffset() const;
	glm::uint getRowOffset() const;
	glm::vec2 getTileSize() const;
	Floor();
	~Floor();
};

