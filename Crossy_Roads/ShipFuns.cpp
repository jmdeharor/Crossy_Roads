#include "FloorRow.h"
#include "Utils.h"
using namespace glm;

inline bool applyConstraints(uint meshIndex, uint numAdjacents, vector<uint>& adjacentRow, uint position) {
	if (position + numAdjacents < adjacentRow.size() && adjacentRow[position + numAdjacents - 1] != adjacentRow[position + numAdjacents])
		return false;
	if (meshIndex == adjacentRow[position])
		return false;
	return true;
}

inline pair<uint, uint> generateRandomTextureIndex(FloorResources& res, uint i, uint prevMeshIndex, vector<uint>& adjacentRow) {
	uint textureIndex;
	uint numAdjacentTiles;
	vector<uint> indicesCopy = res.indices;
	while (indicesCopy.size() > 0) {
		uint ind = rand() % indicesCopy.size();
		textureIndex = indicesCopy[ind];
		indicesCopy.erase(indicesCopy.begin() + ind);
		if (prevMeshIndex == textureIndex)
			continue;
		for (numAdjacentTiles = res.models[textureIndex].size(); numAdjacentTiles >= 4; --numAdjacentTiles) {
			if (applyConstraints(textureIndex, numAdjacentTiles, adjacentRow, i)) {
				indicesCopy.clear();
				break;
			}
		}
	}
	return make_pair(textureIndex, numAdjacentTiles);
}

void FloorRow::initShipRoad(vector<uint>& adjacentRow) {
	animatedFloorTiles.clear();
	platforms.clear();
	enemies.resize(between(1, 3));
	speeds.resize(enemies.size());
	floorTiles.resize(fp.cols);

	static float realTileSize = fp.tileSize.x / fp.cols;
	const static Mesh* cubeMesh = res.assets->getCubeMesh();
	static vec3 boundingBox = cubeMesh->getbbSize();
	static vec3 bbcenter = cubeMesh->getbbCenter();
	static float height = cubeMesh->getHeight();
	vec3 floorTileSize = vec3(realTileSize, 0.1f, fp.tileSize.y) / boundingBox;
	float offsetX = pos.x - (realTileSize*(fp.cols / 2) - (1 - fp.cols % 2)*realTileSize / 2);
	
	enemies[0].horizontalSpeed = generateSpeed();

	for (uint i = 1; i < enemies.size(); ++i) {
		enemies[i].horizontalSpeed = 0;
	}

	uint minTime = uint(fp.realTileSize / abs(enemies[0].horizontalSpeed)) + 1;
	uint maxTime = uint((fp.tileSize.x - fp.realTileSize) / abs(enemies[0].horizontalSpeed));
	frameLimit = between(minTime + 10, int(maxTime*0.4f));
	frameCounter = 0;

	for (uint i = 0; i < enemies.size(); ++i) {
		Jumper& enemy = enemies[i];
		IdMesh enemyId = res.groups[sub2ind(biome, Enemy)][rand()% res.groups[sub2ind(biome, Enemy)].size()];
		enemy.setMesh(enemyId, res.assets->getMesh(enemyId));
		enemy.setScale(vec3(0.1f));
		enemy.setCenterToBaseCenter();
		enemy.setPlane(vec4(0, 1, 0, -rowHeight), fp.lightDir);
		float startPoint;
		if (enemy.horizontalSpeed >= 0) {
			enemy.setRotationY(PI / 2);
			startPoint = fp.lowerLimit;
		}
		else {
			enemy.setRotationY(-PI / 2);
			startPoint = fp.upperLimit;
		}
		enemy.setPos(vec3(startPoint, rowHeight, pos.y));
	}

	uint textureIndex = res.models.size();
	IdTex texture = INVALID;
	uint numAdjacentTiles = 0;
	uint counter = numAdjacentTiles;
	vector<uint> indicesAux;
	for (uint i = 0; i < floorTiles.size(); ++i) {
		TexturedObject& tile = floorTiles[i];
		if (counter == numAdjacentTiles) {
			pair<uint, uint> ret = generateRandomTextureIndex(res, i, textureIndex, adjacentRow);
			textureIndex = ret.first;
			numAdjacentTiles = ret.second;
			counter = 0;
		}
		texture = res.floorTextures[textureIndex][counter];
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

void FloorRow::initShipSafeZone(const FloorRow& prevRow) {
	animatedFloorTiles.clear();
	platforms.clear();
	enemies.clear();
	speeds.clear();
	switch (prevRow.biome) {
	case Ship:
		if (prevRow.safeZone) rowHeight = prevRow.rowHeight;
		else rowHeight = prevRow.rowHeight + 0.2f;
		break;
	case Sea:
		rowHeight = prevRow.rowHeight + 5;
		break;
	case Island:
		break;
	}

	static float realTileSize = fp.tileSize.x / fp.cols;
	float offsetX = pos.x - (realTileSize*(fp.cols / 2) - (1 - fp.cols % 2)*fp.realTileSize / 2);

	const static Mesh* cubeMesh = res.assets->getCubeMesh();
	static vec3 boundingBox = cubeMesh->getbbSize();
	static vec3 bbcenter = cubeMesh->getbbCenter();
	static float height = cubeMesh->getHeight();

	bool hasEmpty = false;
	for (uint i = 0; i < map.size(); ++i) {
		if (map[i].empty)
			hasEmpty = true;
		if (map[i].mesh == INVALID)
			continue;
		ShadowedObject object;
		IdMesh meshId = map[i].mesh;
		float height = map[i].height;

		const Mesh* mesh = res.assets->getMesh(meshId);

		vec3 boundingBox = mesh->getbbSize();
		vec3 bbCenter = mesh->getbbCenter();
		vec3 objectSize = vec3(realTileSize*map[i].cols, height, 0) / boundingBox;
		objectSize.z = objectSize.x;
		object.name = "Ship furniture " + to_string(i);
		object.setMesh(meshId, mesh);
		object.setScale(objectSize);
		object.setCenterToBaseCenter();
		float posX = offsetX + i*realTileSize;
		object.setPos(vec3(posX + (realTileSize*(map[i].cols / 2.f)) - realTileSize / 2, rowHeight, pos.y - fp.tileSize.y*(map[i].rows / 2.f) + fp.tileSize.y / 2));
		object.setPlane(vec4(0, 1, 0, -rowHeight), fp.lightDir);
		furniture.push_back(object);
	}

	if (hasEmpty) {
		static vec3 floorTileSize = vec3(fp.realTileSize, 0.2f, fp.tileSize.y) / boundingBox;
		floorTiles.resize(fp.cols);

		for (uint i = 0; i < floorTiles.size(); ++i) {
			TexturedObject& tile = floorTiles[i];
			if (map[i].empty)
				tile.texture = res.water;
			else
				tile.texture = res.planeWood;
			tile.setMesh(cubeMesh);
			tile.setScale(floorTileSize);
			tile.setCenter(vec3(bbcenter.x, bbcenter.y + height / 2.f, bbcenter.z));
			tile.setPos(vec3(offsetX + i*fp.realTileSize, rowHeight, pos.y));
		}
	}
	else {
		static vec3 floorTileSize = vec3(fp.tileSize.x, 0.2f, fp.tileSize.y) / boundingBox;
		floorTiles.resize(1);

		TexturedObject& tile = floorTiles[0];
		tile.texture = res.planeWood;
		tile.setMesh(cubeMesh);
		tile.setScale(floorTileSize);
		tile.setCenter(vec3(bbcenter.x, bbcenter.y + height / 2.f, bbcenter.z));
		tile.setPos(vec3(pos.x, rowHeight, pos.y));
	}
	
}