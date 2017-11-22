#pragma once
#include "GameObject.h"
#include "ShadowedObject.h"
#include "ImportedMesh.h"
#include "CubeMesh.h"
#include "TexturedObject.h"

class FloorRow : public GameObject {
	std::vector<TexturedObject> floorTiles;
	ShadowedObject mast;
	bool hasMast;
	std::vector<ShadowedObject> enemies;
	std::vector<float> speeds;
	glm::vec2 pos;
	static glm::vec2 tileSize;
	static uint cols;
	static glm::vec3 lightDir;
	static ImportedMesh pirateMesh;
	static ImportedMesh mastMesh;
	static CubeMesh floorMesh;
	static std::vector<Texture> floorTextures;
public:
	static void initMeshes();
	static void setParameters(glm::vec2 tileSize, uint cols, glm::vec3 lightDir);
	glm::vec2 getPos() const;
	void moveToPosition(glm::vec2 position);
	void setPos(glm::vec2 position);
	void init(vector<uint>& startPositions);
	void update(int deltaTime);
	void renderSimpleObjects(ShaderProgram& program);
	void renderLightObjects(ShaderProgram& program);
	void renderShadows(ShaderProgram& program);
	FloorRow();
	~FloorRow();
};

