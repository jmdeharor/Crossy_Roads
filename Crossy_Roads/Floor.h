#pragma once
#include "QuadMesh.h"
#include <vector>
#include "ShadowedObject.h"
#include "Renderer.h"
#include "FloorRow.h"

class Floor : GameObject {
	uint rows, cols;
	uint lastRow;
	float firstPos;
	glm::vec3 lightDir;
	Mesh floorMesh;
	std::vector<FloorRow> floorRows;
	glm::vec2 tileSize;
public:
	void addLevel();
	void firstInit();
	void setLight(glm::vec3 lightDir);
	void init();
	void update(int deltaTime);
	void renderSimpleObjects(ShaderProgram& program);
	void renderLightObjects(ShaderProgram& program);
	void renderShadows(ShaderProgram& program);
	glm::vec2 getTileSize();
	Floor();
	~Floor();
};

