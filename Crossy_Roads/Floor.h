#pragma once
#include "QuadMesh.h"
#include <vector>
#include "ShadowedObject.h"
#include "ImportedMesh.h"
#include "FloorRow.h"

class Floor : GameObject {
	glm::uint rows, cols;
	glm::uint lastRow;
	float firstPos;
	CubeMesh cubeMesh;
	ImportedMesh deckMesh[4];
	std::vector<FloorRow> floorRows;
	glm::vec2 tileSize;
public:
	void addLevel();
	void firstInit() override;
	void init(glm::vec3 lightDir);
	void update(int deltaTime);
	void renderSimpleObjects(ShaderProgram& program);
	void renderLightObjects(ShaderProgram& program);
	void renderShadows(ShaderProgram& program);
	const FloorRow* getFloorRow(glm::uint index) const;
	glm::uint getRows() const;
	glm::vec2 getTileSize();
	Floor();
	~Floor();
};

