#pragma once
#include "QuadMesh.h"
#include <vector>
#include "ShadowedObject.h"
#include "ImportedMesh.h"
#include "Renderer.h"
#include "FloorRow.h"

class Floor : GameObject {
	uint rows, cols;
	uint lastRow;
	float firstPos;
	ImportedMesh floorMesh;
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
	glm::vec2 getTileSize();
	Floor();
	~Floor();
};

