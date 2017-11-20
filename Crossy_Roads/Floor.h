#pragma once
#include "QuadMesh.h"
#include <vector>
#include "ShadowedObject.h"
#include "Renderer.h"

#define PI 3.14159f

class Floor : GameObject {
	Mesh enemyMesh;
	uint rows, cols;
	uint lastRow;
	float firstPos;
	glm::vec3 lightDir;
	QuadMesh floorPlane;
	std::vector<Object> planeTiles;
	std::vector<ShadowedObject> enemies;
	std::vector<float> velocities;
	Texture floorTex;
	glm::vec2 tileSize;
public:
	glm::vec2 getOffsets();
	uint getNumRows();
	void addLevel();
	void firstInit();
	glm::vec2 getTileSize();
	void setLight(glm::vec3 lightDir);
	void init();
	void addObjects(Renderer& renderer);
	void update(int deltaTime);
	void renderFloor(ShaderProgram& program);
	void renderEnemies(ShaderProgram& program);
	void renderEnemyShadows(ShaderProgram& program);
	Floor();
	~Floor();
};

