#include "FloorRow.h"
#include "Scene.h"
#include "Utils.h"
#include "ShipBiome.h"
#include "SeaBiome.h"
using namespace glm;

union {
	float f;
	struct {
		unsigned int mantisa : 23;
		unsigned int exponent : 8;
		unsigned int sign : 1;
	} parts;
} floatCast;

float FloorRow::worldToCol(float x) {
	return (x - offset) / fp.realTileSize;
}

vec2 FloorRow::getPos() const {
	return pos;
}

void FloorRow::setPos(vec2 position) {
	pos = position;
}

void FloorRow::initIds(const Assets & assets) {
	Biome::initIds(assets);
}

void FloorRow::initRoad(BiomeType type, vector<uint>& adjacentRow, const vector<CellProperties>& map) {
	v.rowObjects = map;
	biomeType = type;
	safeZone = false;
	if (biome != NULL) delete biome;
	switch (type) {
	case Ship:
		biome = new ShipBiome(v);
		break;
	case Sea:
		biome = new SeaBiome(v);
		break;
	}
	biome->pos = pos;
	biome->initRoad(adjacentRow, map);
	rowHeight = biome->rowHeight;
}

void FloorRow::initSafeZone(BiomeType type, const vector<CellProperties>& map) {
	v.rowObjects = map;
	biomeType = type;
	safeZone = true;
	if (biome != NULL) delete biome;
	switch (type) {
	case Ship:
		biome = new ShipBiome(v);
		break;
	case Sea:
		biome = new SeaBiome(v);
		break;
	}
	biome->pos = pos;
	biome->initSafeZone(map);
	rowHeight = biome->rowHeight;
}

void FloorRow::setParameters(const FloorParams & floorParams) {
	fp = floorParams;
	offset = -fp.tileSize.x / 2 + fp.colOffset*fp.realTileSize;
	Biome::setParameters(floorParams);
}

pair<vec3, float> FloorRow::getHeight(uint col) {
	float offsetX = pos.x - (fp.realTileSize*(fp.cols / 2) - (1 - fp.cols % 2)*fp.realTileSize / 2);
	vec3 myHeight = vec3(offsetX + col*fp.realTileSize, rowHeight, pos.y);
	float platformSpeed = 0;
	for (uint i = 0; i < v.platforms.size(); ++i) {
		uint index = worldToCol(v.platforms[i].getPos().x);
		if (index == col || index == col + 1 || index == col - 1) {
			myHeight.y = v.platforms[i].getPos().y + v.platforms[i].getHeight();
			myHeight.x = v.platforms[i].getPos().x + v.speeds[i] * 11;
			if (index == col + 1) myHeight.x -= fp.realTileSize;
			else if (index == col - 1) myHeight.x += fp.realTileSize;
			platformSpeed = v.speeds[i];
			break;
		}
	}
	return make_pair(myHeight, platformSpeed);
}

void FloorRow::update(int deltaTime) {
	for (uint i = 0; i < v.enemies.size(); ++i) {
		Jumper& object = v.enemies[i];
		object.update();
		float x = object.getPos().x;
		if (x > fp.tileSize.x / 2 || x < -fp.tileSize.x/2) {
			object.horizontalSpeed = generateSpeed();
			float startPoint;
			if (object.horizontalSpeed >= 0) {
				object.setRotationY(PI/2);
				startPoint = -fp.tileSize.x / 2;
			}
			else {
				object.setRotationY(-PI/2);
				startPoint = fp.tileSize.x / 2;
			}
			object.setPos(vec3(startPoint, rowHeight, pos.y));
		}
		if (rand() % 512 == 0) {
			object.jump();
		}
	}
	for (uint i = 0; i < v.platforms.size(); ++i) {
		Object& object = v.platforms[i];
		object.move(v.speeds[i], 0, 0);
		float x = object.getPos().x;
		if (x > fp.tileSize.x / 2 || x < -fp.tileSize.x / 2) {
			float upperLimit;
			if (i == 0) {
				upperLimit = 0.2f;
			}
			else {
				upperLimit = abs(v.speeds[i - 1]);
			}
			floatCast.f = v.speeds[i];
			v.speeds[i] = generateSpeed(0.15f / (i+1), upperLimit, !floatCast.parts.sign);

			float startPoint;
			if (v.speeds[i] >= 0) {
				object.setRotationY(0);
				startPoint = -fp.tileSize.x / 2;
			}
			else {
				object.setRotationY(PI);
				startPoint = fp.tileSize.x / 2;
			}
			vec3 prevPos = object.getPos();
			object.setPos(vec3(startPoint, prevPos.y, pos.y));
		}
	}
}

void FloorRow::groupDrawableObjects(vector<vector<Object*>>& objects, vector<vector<TexturedObject*>>& texturedObjects, const FrustumG& frustum) {
	for (uint i = 0; i < v.enemies.size(); ++i) {
		if (v.enemies[i].isInsideViewFrustrum(frustum))
			objects[v.enemies[i].meshId].push_back(&v.enemies[i]);
	}
	for (uint i = 0; i < v.floorTiles.size(); ++i) {
		if (v.floorTiles[i].isInsideViewFrustrum(frustum))
			texturedObjects[v.floorTiles[i].texture].push_back(&v.floorTiles[i]);
	}
	for (uint i = 0; i < v.furniture.size(); ++i) {
		if (v.furniture[i].isInsideViewFrustrum(frustum))
			objects[v.furniture[i].meshId].push_back(&v.furniture[i]);
	}
	for (uint i = 0; i < v.platforms.size(); ++i) {
		if (v.platforms[i].isInsideViewFrustrum(frustum)) {
			objects[v.platforms[i].meshId].push_back(&v.platforms[i]);
		}
	}
}

vector<Jumper>* FloorRow::getEnemies() {
	return &v.enemies;
}

vector<CellProperties>* FloorRow::getRowObjects() {
	return &v.rowObjects;
}

vector<ShadowedObject>* FloorRow::getPlatforms()
{
	return &v.platforms;
}

bool FloorRow::isSafeZone() const {
	return safeZone;
}
BiomeType FloorRow::getBiome() const {
	return biomeType;
}

FloorRow::FloorRow() : biome(NULL) {
}


FloorRow::~FloorRow()
{
}

float FloorRow::offset;
FloorParams FloorRow::fp;