#pragma once
#include "GameObject.h"
#include "ShadowedObject.h"
#include "ImportedMesh.h"
#include "CubeMesh.h"
#include "TexturedObject.h"

enum FloorType {
	Road,
	Safe
};

class FloorRow : public GameObject {
private:
	std::vector<TexturedObject> floorTiles;
	std::vector<ShadowedObject> enemies;
	std::vector<float> speeds;
	glm::vec2 pos;
	static glm::vec2 tileSize;
	static uint cols;
	static glm::vec3 lightDir;
	static ImportedMesh pirateMesh;
	static CubeMesh cubeMesh;
	static std::vector<vector<Texture>> floorTextures;
	static Texture planeWood;
	float rowHeight;
public:
	static void initMeshes();
	static void setParameters(glm::vec2 tileSize, uint cols, glm::vec3 lightDir);
	glm::vec2 getPos() const;
	void moveToPosition(glm::vec2 position);
	void setPos(glm::vec2 position);
	void initSafeZone();
	void initRoad(vector<uint>& adjacentRow);
	void update(int deltaTime);
	void renderSimpleObjects(ShaderProgram& program);
	void renderLightObjects(ShaderProgram& program);
	void renderShadows(ShaderProgram& program);
	float getHeight();
	FloorRow();
	~FloorRow();
};

