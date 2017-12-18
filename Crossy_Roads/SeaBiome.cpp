#include "SeaBiome.h"
#include "Utils.h"
using namespace glm;

void SeaBiome::initRoad(std::vector<glm::uint>& adjacentRow, const std::vector<CellProperties>& map) {
	v.furniture.clear();
	v.enemies.clear();
	v.platforms.resize(between(2, 4));
	v.speeds.resize(v.platforms.size());
	v.rowObjects.clear();
	v.floorTiles.resize(fp.cols);
	rowHeight = -5;

	static float realTileSize = fp.tileSize.x / fp.cols;
	float offsetX = pos.x - (realTileSize*(fp.cols / 2) - (1 - fp.cols % 2)*realTileSize / 2);

	static const Mesh* cubeMesh = assets->getCubeMesh();
	static vec3 boundingBox = cubeMesh->getbbSize();
	static vec3 bbcenter = cubeMesh->getbbCenter();
	static float height = cubeMesh->getHeight();
	static vec3 floorTileSize = vec3(realTileSize, 0.2f, fp.tileSize.y) / boundingBox;

	for (uint i = 0; i < v.floorTiles.size(); ++i) {
		TexturedObject& tile = v.floorTiles[i];
		tile.texture = water;
		tile.setRotationY(PI / 2);
		tile.setMesh(cubeMesh);
		tile.setScale(floorTileSize);
		tile.setCenter(vec3(bbcenter.x, bbcenter.y + height / 2.f, bbcenter.z));
		tile.setPos(vec3(offsetX + i*realTileSize, rowHeight, pos.y));
	}

	static const Mesh* crocoMesh = assets->getMesh(crocodile);
	static const Mesh* sharkMesh = assets->getMesh(shark);
	static vec3 crocodilebb = crocoMesh->getbbSize();
	static vec3 crocoSize = vec3(3 * realTileSize, 1, fp.tileSize.y) / crocodilebb;

	uint nPlat = v.platforms.size();
	for (uint i = 0; i < nPlat; ++i) {
		ShadowedObject& platform = v.platforms[i];
		platform.setMesh(shark, sharkMesh);
		platform.setScale(crocoSize);
		platform.setCenterToBaseCenter();
		platform.setPlane(vec4(0, 1, 0, -rowHeight), fp.lightDir);
		float upperLimit;
		if (i == 0) {
			upperLimit = 0.2f;
		}
		else {
			upperLimit = abs(v.speeds[i - 1]);
		}
		v.speeds[i] = generateSpeed(0.1f / (i + 1), upperLimit, int(pos.y / fp.tileSize.y) % 2);
		float startPoint;
		if (v.speeds[i] >= 0) {
			platform.setRotationY(0);
			startPoint = -fp.tileSize.x / 2;
		}
		else {
			platform.setRotationY(PI);
			startPoint = fp.tileSize.x / 2;
		}
		platform.setPos(vec3(startPoint, rowHeight - 0.5f, pos.y));
	}

	for (uint i = 0; i < map.size(); ++i) {
		if (map[i].height == 0 || map[i].mesh == INVALID)
			continue;
		v.furniture.push_back(ShadowedObject());
		ShadowedObject& object = v.furniture[v.furniture.size() - 1];
		IdMesh meshId = map[i].mesh;
		float height = map[i].height;

		const Mesh* mesh = assets->getMesh(meshId);

		vec3 boundingBox = mesh->getbbSize();
		vec3 bbCenter = mesh->getbbCenter();
		vec3 objectSize = vec3(realTileSize*map[i].cols, height, fp.tileSize.y*map[i].rows) / boundingBox;

		object.setMesh(meshId, mesh);
		object.setScale(objectSize);
		object.setCenterToBaseCenter();
		float posX = offsetX + i*realTileSize;
		object.setPos(vec3(posX + (realTileSize*(map[i].cols / 2.f)) - realTileSize / 2, rowHeight, pos.y - fp.tileSize.y*(map[i].rows / 2.f) + fp.tileSize.y / 2));
		object.setPlane(vec4(0, 1, 0, -rowHeight), fp.lightDir);
	}
}

SeaBiome::SeaBiome(RowVectors& vectors) : Biome(vectors) {
}


SeaBiome::~SeaBiome()
{
}
