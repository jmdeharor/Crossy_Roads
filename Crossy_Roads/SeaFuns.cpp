#include "Floor.h"
#include "Utils.h"
using namespace glm;

void FloorRow::initSeaRoad(vector<uint>& adjacentRow) {
	furniture.clear();
	enemies.clear();
	platforms.resize(between(2, 4));
	speeds.resize(platforms.size());
	floorTiles.resize(fp.cols);
	rowHeight = -5;

	float offsetX = pos.x - (fp.realTileSize*(fp.cols / 2) - (1 - fp.cols % 2)*fp.realTileSize / 2);

	static const Mesh* cubeMesh = res.assets->getCubeMesh();
	static vec3 boundingBox = cubeMesh->getbbSize();
	static vec3 bbcenter = cubeMesh->getbbCenter();
	static float height = cubeMesh->getHeight();
	static vec3 floorTileSize = vec3(fp.realTileSize, 0.2f, fp.tileSize.y) / boundingBox;

	for (uint i = 0; i < floorTiles.size(); ++i) {
		TexturedObject& tile = floorTiles[i];
		tile.texture = res.water;
		tile.setRotationY(PI / 2);
		tile.setMesh(cubeMesh);
		tile.setScale(floorTileSize);
		tile.setCenter(vec3(bbcenter.x, bbcenter.y + height / 2.f, bbcenter.z));
		tile.setPos(vec3(offsetX + i*fp.realTileSize, rowHeight, pos.y));
	}

	static const Mesh* sharkMesh = res.assets->getMesh(res.shark);
	static vec3 sharkbb = sharkMesh->getbbSize();
	static vec3 sharkSize = vec3(3 * fp.realTileSize, 1, fp.tileSize.y+1) / sharkbb;

	speeds[0] = generateSpeed(minSpeed, maxSpeed, int(pos.y / fp.tileSize.y) % 2);

	for (uint i = 1; i < speeds.size(); ++i) {
		speeds[i] = 0;
	}

	uint minTime = uint(3 * fp.realTileSize / abs(speeds[0])) + 1;
	uint maxTime = uint((fp.tileSize.x - 3 * fp.realTileSize) / abs(speeds[0]));
	frameLimit = between(minTime + 10, int(maxTime*0.3f));
	frameCounter = 0;

	uint nPlat = platforms.size();
	for (uint i = 0; i < nPlat; ++i) {
		ShadowedObject& platform = platforms[i];
		platform.setMesh(res.shark, sharkMesh);
		platform.setScale(sharkSize);
		platform.setCenterToBaseCenter();
		platform.setPlane(vec4(0, 1, 0, -rowHeight), fp.lightDir);
		float startPoint;
		if (speeds[0] >= 0) {
			platform.setRotationY(0);
			startPoint = -fp.tileSize.x / 2;
		}
		else {
			platform.setRotationY(PI);
			startPoint = fp.tileSize.x / 2;
		}
		platform.setPos(vec3(startPoint, rowHeight -0.3f, pos.y));
	}

	for (uint i = 0; i < map.size(); ++i) {
		if (map[i].mesh == INVALID)
			continue;
		furniture.push_back(ShadowedObject());
		ShadowedObject& object = furniture[furniture.size() - 1];
		IdMesh meshId = map[i].mesh;
		float height = map[i].height;

		const Mesh* mesh = res.assets->getMesh(meshId);

		vec3 boundingBox = mesh->getbbSize();
		vec3 bbCenter = mesh->getbbCenter();
		vec3 objectSize = vec3(fp.realTileSize*map[i].cols, height, fp.tileSize.y*map[i].rows) / boundingBox;

		object.setMesh(meshId, mesh);
		object.setScale(objectSize);
		object.setCenterToBaseCenter();
		float posX = offsetX + i*fp.realTileSize;
		object.setPos(vec3(posX + (fp.realTileSize*(map[i].cols / 2.f)) - fp.realTileSize / 2, rowHeight+map[i].verticalOffset, pos.y - fp.tileSize.y*(map[i].rows / 2.f) + fp.tileSize.y / 2));
		object.setPlane(vec4(0, 1, 0, -rowHeight), fp.lightDir);
	}
}