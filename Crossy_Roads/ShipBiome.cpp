#include "ShipBiome.h"
#include "Utils.h"
using namespace glm;

bool applyConstraints(uint meshIndex, uint numAdjacents, vector<uint>& adjacentRow, uint position) {
	if (position + numAdjacents < adjacentRow.size() && adjacentRow[position + numAdjacents - 1] != adjacentRow[position + numAdjacents])
		return false;
	if (meshIndex == adjacentRow[position])
		return false;
	return true;
}

pair<uint, uint> generateRandomTextureIndex(uint i, uint prevMeshIndex, vector<uint>& adjacentRow) {
	uint textureIndex;
	uint numAdjacentTiles;
	vector<uint> indicesCopy = Biome::indices;
	while (indicesCopy.size() > 0) {
		uint ind = rand() % indicesCopy.size();
		textureIndex = indicesCopy[ind];
		indicesCopy.erase(indicesCopy.begin() + ind);
		if (prevMeshIndex == textureIndex)
			continue;
		for (numAdjacentTiles = Biome::models[textureIndex].size(); numAdjacentTiles >= 4; --numAdjacentTiles) {
			if (applyConstraints(textureIndex, numAdjacentTiles, adjacentRow, i)) {
				indicesCopy.clear();
				break;
			}
		}
	}
	return make_pair(textureIndex, numAdjacentTiles);
}

void ShipBiome::initRoad(vector<uint>& adjacentRow, const vector<CellProperties>& map) {
	v.rowObjects.clear();
	v.furniture.clear();
	v.platforms.clear();
	v.enemies.resize(2);
	v.speeds.resize(v.enemies.size());
	v.floorTiles.resize(fp.cols);
	rowHeight = 0;

	static float realTileSize = fp.tileSize.x / fp.cols;
	const static Mesh* cubeMesh = assets->getCubeMesh();
	static vec3 boundingBox = cubeMesh->getbbSize();
	static vec3 bbcenter = cubeMesh->getbbCenter();
	static float height = cubeMesh->getHeight();
	vec3 floorTileSize = vec3(realTileSize, 0.1f, fp.tileSize.y) / boundingBox;
	float offsetX = pos.x - (realTileSize*(fp.cols / 2) - (1 - fp.cols % 2)*realTileSize / 2);

	for (uint i = 0; i < v.enemies.size(); ++i) {
		Jumper& enemy = v.enemies[i];
		enemy.setMesh(enemyMeshes[i], assets->getMesh(enemyMeshes[i]));
		enemy.setScale(vec3(0.1f));
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

	uint textureIndex = models.size();
	IdTex texture = INVALID;
	uint numAdjacentTiles = 0;
	uint counter = numAdjacentTiles;
	vector<uint> indicesAux;
	for (uint i = 0; i < v.floorTiles.size(); ++i) {
		TexturedObject& tile = v.floorTiles[i];
		if (counter == numAdjacentTiles) {
			pair<uint, uint> ret = generateRandomTextureIndex(i, textureIndex, adjacentRow);
			textureIndex = ret.first;
			numAdjacentTiles = ret.second;
			counter = 0;
		}
		texture = floorTextures[textureIndex][counter];
		adjacentRow[i] = textureIndex;
		tile.texture = texture;
		tile.setMesh(cubeMesh);
		tile.setScale(floorTileSize);
		tile.setRotationY(PI / 2);
		tile.setCenter(vec3(bbcenter.x, bbcenter.y + height / 2.f, bbcenter.z));
		tile.setPos(vec3(offsetX + i*realTileSize, rowHeight, pos.y));
		++counter;
	}
}

void ShipBiome::initSafeZone(const vector<CellProperties>& map) {
	v.platforms.clear();
	v.enemies.clear();
	v.speeds.clear();
	v.rowObjects = map;
	v.floorTiles.resize(1);
	rowHeight = 0.2f;

	static float realTileSize = fp.tileSize.x / fp.cols;
	float offsetX = pos.x - (realTileSize*(fp.cols / 2) - (1 - fp.cols % 2)*fp.realTileSize / 2);

	const static Mesh* cubeMesh = assets->getCubeMesh();
	static vec3 boundingBox = cubeMesh->getbbSize();
	static vec3 bbcenter = cubeMesh->getbbCenter();
	static float height = cubeMesh->getHeight();
	static vec3 floorTileSize = vec3(fp.tileSize.x, 0.2f, fp.tileSize.y) / boundingBox;

	TexturedObject& tile = v.floorTiles[0];
	tile.texture = planeWood;
	tile.setMesh(cubeMesh);
	tile.setScale(floorTileSize);
	tile.setCenter(vec3(bbcenter.x, bbcenter.y + height / 2.f, bbcenter.z));
	tile.setPos(vec3(pos.x, rowHeight, pos.y));

	v.furniture.clear();
	for (uint i = 0; i < fp.cols; ++i) {
		if (map[i].height == 0 || map[i].mesh == INVALID)
			continue;
		v.furniture.push_back(ShadowedObject());
		ShadowedObject& object = v.furniture[v.furniture.size() - 1];
		IdMesh meshId = map[i].mesh;
		float height = map[i].height;

		const Mesh* mesh = assets->getMesh(meshId);

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
}

ShipBiome::ShipBiome(RowVectors& vectors) : Biome(vectors) {
}


ShipBiome::~ShipBiome()
{
}
