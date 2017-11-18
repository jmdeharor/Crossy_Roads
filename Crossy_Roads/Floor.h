#pragma once
#include "QuadMesh.h"
#include <vector>
#include "Object.h"

class Floor : GameObject {
	uint rows, cols;
	uint lastRow;
	float firstPos;
	QuadMesh floorPlane;
	std::vector<Object> planeTiles;
	ShaderProgram* program;
	Texture floorTex;
	glm::vec2 tileSize;
public:
	glm::vec2 getOffsets();
	uint getNumRows();
	void addLevel();
	void setShaderProgram(ShaderProgram* shaderProgram);
	void firstInit();
	glm::vec2 getTileSize();
	void init();
	void render();
	void update(int deltaTime);
	Floor();
	~Floor();
};

