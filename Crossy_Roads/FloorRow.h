#pragma once
#include "GameObject.h"
#include "ShadowedObject.h"
#include "ImportedMesh.h"
#include "CubeMesh.h"
#include "TexturedObject.h"
#include "ObjectContainer.h"

struct CellProperties {
	IdMesh mesh;
	float height;
};

enum FloorType {
	Road,
	Safe
};

class FloorRow : public ObjectContainer {
private:
	std::vector<ShadowedObject> furniture;
	std::vector<TexturedObject> floorTiles;
	std::vector<ShadowedObject> enemies;
	std::vector<float> speeds;
	glm::vec2 pos;
	const static Assets* assets;
	static glm::vec2 tileSize;
	static glm::uint cols;
	static glm::vec3 lightDir;
	static IdMesh pirateMesh;
	static IdMesh pirateMesh2;
	static std::vector<IdMesh> enemyMeshes;
	const static Mesh* cubeMesh;
	static std::vector<vector<IdTex>> floorTextures;
	static IdMesh planeWood;
	float rowHeight;
public:
	static void initIds(const Assets& assets);
	void initSafeZone(std::vector<CellProperties> map);
	static void setParameters(glm::vec2 tileSize, glm::uint cols, glm::vec3 lightDir);
	glm::vec2 getPos() const;
	void moveToPosition(glm::vec2 position);
	void setPos(glm::vec2 position);
	void initRoad(vector<glm::uint>& adjacentRow);
	void update(int deltaTime);
	void groupDrawableObjects(
		std::vector<std::vector<Object*>>& objects,



		std::vector<std::vector<TexturedObject*>>& texturedObjects, const FrustumG& frustum
	) override;
	void renderSimpleObjects(ShaderProgram& program);
	void renderLightObjects(ShaderProgram& program);
	void renderShadows(ShaderProgram& program);
	float getHeight() const;
	FloorRow();
	~FloorRow();
};

