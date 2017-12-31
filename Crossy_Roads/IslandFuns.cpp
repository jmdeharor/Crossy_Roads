#include "FloorRow.h"
#include "Utils.h"
using namespace glm;

void FloorRow::initIslandRoad() {
	animatedFloorTiles.clear();
	furniture.clear();
	platforms.clear();
	enemies.resize(res.groups[sub2ind(biome, Enemy)].size());
	speeds.resize(enemies.size());
	floorTiles.resize(fp.cols);

	static float realTileSize = fp.tileSize.x / fp.cols;
	const static Mesh* cubeMesh = res.assets->getCubeMesh();
	static vec3 boundingBox = cubeMesh->getbbSize();
	static vec3 bbcenter = cubeMesh->getbbCenter();
	static float height = cubeMesh->getHeight();
	vec3 floorTileSize = vec3(realTileSize, 0.1f, fp.tileSize.y) / boundingBox;
	float offsetX = pos.x - (realTileSize*(fp.cols / 2) - (1 - fp.cols % 2)*realTileSize / 2);

	for (uint i = 0; i < enemies.size(); ++i) {
		Jumper& enemy = enemies[i];
		IdMesh enemyId = res.groups[sub2ind(biome, Enemy)][i];
		enemy.setMesh(enemyId, res.assets->getMesh(enemyId));
		enemy.setScale(vec3(0.05f));
		enemy.setCenterToBaseCenter();
		enemy.setPlane(vec4(0, 1, 0, -rowHeight), fp.lightDir);
		enemy.horizontalSpeed = generateSpeed();
		float startPoint;
		if (enemy.horizontalSpeed >= 0) {
			enemy.setRotationY(PI / 2);
			startPoint = -fp.tileSize.x / 2 + ((float)rand() / RAND_MAX) * fp.tileSize.x;
		}
		else {
			enemy.setRotationY(-PI / 2);
			startPoint = fp.tileSize.x / 2 - ((float)rand() / RAND_MAX) * fp.tileSize.x;
		}
		enemy.setPos(vec3(startPoint, rowHeight, pos.y));
	}

	for (uint i = 0; i < floorTiles.size(); ++i) {
		TexturedObject& tile = floorTiles[i];
		tile.texture = res.islandRoad;
		tile.setMesh(cubeMesh);
		tile.setScale(floorTileSize);
		tile.setRotationY(PI / 2);
		tile.setCenter(vec3(bbcenter.x, bbcenter.y + height / 2.f, bbcenter.z));
		tile.setPos(vec3(offsetX + i*realTileSize, rowHeight, pos.y));
	}
}

void FloorRow::initIslandSafeZone(const FloorRow& prevRow) {
	animatedFloorTiles.clear();
	floorTiles.resize(fp.cols);
	platforms.clear();
	enemies.clear();
	speeds.clear();
	furniture.clear();
	switch (prevRow.biome) {
	case Ship:
		break;
	case Sea:
		rowHeight = prevRow.rowHeight + 0.2f;
		break;
	case Island:
		if (prevRow.safeZone) rowHeight = prevRow.rowHeight;
		else rowHeight = prevRow.rowHeight + 0.2f;
		break;
	}

	static float realTileSize = fp.tileSize.x / fp.cols;
	float offsetX = pos.x - (realTileSize*(fp.cols / 2) - (1 - fp.cols % 2)*fp.realTileSize / 2);

	const static Mesh* cubeMesh = res.assets->getCubeMesh();
	static vec3 boundingBox = cubeMesh->getbbSize();
	static vec3 bbcenter = cubeMesh->getbbCenter();
	static float height = cubeMesh->getHeight();

	for (uint i = 0; i < fp.cols; ++i) {
		if (map[i].mesh == INVALID)
			continue;
		furniture.push_back(ShadowedObject());
		ShadowedObject& object = furniture[furniture.size() - 1];
		IdMesh meshId = map[i].mesh;
		float height = map[i].height;

		const Mesh* mesh = res.assets->getMesh(meshId);

		vec3 boundingBox = mesh->getbbSize();
		vec3 bbCenter = mesh->getbbCenter();
		vec3 objectSize = vec3(realTileSize*map[i].cols, height, 0) / boundingBox;
		objectSize.z = objectSize.x;
		object.setMesh(meshId, mesh);
		object.setScale(objectSize);
		object.setCenterToBaseCenter();
		float posX = offsetX + i*realTileSize;
		object.setPos(vec3(posX + (realTileSize*(map[i].cols / 2.f)) - realTileSize / 2, rowHeight, pos.y - fp.tileSize.y*(map[i].rows / 2.f) + fp.tileSize.y / 2));
		object.setPlane(vec4(0, 1, 0, -rowHeight), fp.lightDir);
	}
	
	static vec3 floorTileSize = vec3(fp.realTileSize, 0.2f, fp.tileSize.y) / boundingBox;

	for (uint i = 0; i < floorTiles.size(); ++i) {
		TexturedObject& tile = floorTiles[i];
		tile.texture = res.islandSafe;
		tile.setMesh(cubeMesh);
		tile.setScale(floorTileSize);
		tile.setRotationY(PI / 2);
		tile.setCenter(vec3(bbcenter.x, bbcenter.y + height / 2.f, bbcenter.z));
		tile.setPos(vec3(offsetX + i*realTileSize, rowHeight, pos.y));
	}

}