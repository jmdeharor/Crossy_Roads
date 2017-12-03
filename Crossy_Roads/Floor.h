#pragma once
#include "QuadMesh.h"
#include <vector>
#include "ShadowedObject.h"
#include "ImportedMesh.h"
#include "FloorRow.h"
#include "Assets.h"
#include "ObjectContainer.h"

class Floor : public GameObject, public ObjectContainer {
	glm::uint rows, cols;
	glm::uint lastRow;
	IdMesh deckMesh[4];
	float firstPos;
	std::vector<FloorRow> floorRows;
	glm::vec2 tileSize;
public:
	void addLevel();
	void firstInit() override;
	void init(glm::vec3 lightDir, const Assets& assets);
	void update(int deltaTime);
	void groupDrawableObjects(
		std::vector<std::vector<Object*>>& objects,
		std::vector<std::vector<TexturedObject*>>& texturedObjects
	) override;
	void renderSimpleObjects(ShaderProgram& program);
	void renderLightObjects(ShaderProgram& program);
	void renderShadows(ShaderProgram& program);
	const FloorRow* getFloorRow(glm::uint index) const;
	glm::uint getRows() const;
	glm::vec2 getTileSize();
	Floor();
	~Floor();
};

