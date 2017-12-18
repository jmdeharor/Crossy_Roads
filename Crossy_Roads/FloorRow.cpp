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

uint FloorRow::worldToCol(float x) {
	return (x - offset) / fp.realTileSize;
}

void FloorRow::initResources(const Assets & assets) {
	res.init(assets);
}

void FloorRow::initRoad(BiomeType type, vector<uint>& adjacentRow, const vector<CellProperties>& map) {
	rowObjects = map;
	biomeType = type;
	safeZone = false;
	switch (type) {
	case Ship:
		initShipRoad(adjacentRow, map);
		break;
	case Sea:
		initSeaRoad(adjacentRow, map);
		break;
	}
}

void FloorRow::initSafeZone(BiomeType type, const vector<CellProperties>& map) {
	rowObjects = map;
	biomeType = type;
	safeZone = true;
	initShipSafeZone(map);
}

void FloorRow::setParameters(const FloorParams & floorParams) {
	fp = floorParams;
	offset = -fp.tileSize.x / 2 + fp.colOffset*fp.realTileSize;
}

pair<vec3, float> FloorRow::getHeight(uint col) {
	float offsetX = pos.x - (fp.realTileSize*(fp.cols / 2) - (1 - fp.cols % 2)*fp.realTileSize / 2);
	vec3 myHeight = vec3(offsetX + col*fp.realTileSize, rowHeight, pos.y);
	float platformSpeed = 0;
	for (uint i = 0; i < platforms.size(); ++i) {
		uint index = worldToCol(platforms[i].getPos().x);
		if (index == col || index == col + 1 || index == col - 1) {
			myHeight.y = platforms[i].getPos().y + platforms[i].getHeight();
			myHeight.x = platforms[i].getPos().x + speeds[i] * 11;
			if (index == col + 1) myHeight.x -= fp.realTileSize;
			else if (index == col - 1) myHeight.x += fp.realTileSize;
			platformSpeed = speeds[i];
			break;
		}
	}
	return make_pair(myHeight, platformSpeed);
}

void FloorRow::update(int deltaTime) {
	for (uint i = 0; i < enemies.size(); ++i) {
		Jumper& object = enemies[i];
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
	for (uint i = 0; i < platforms.size(); ++i) {
		Object& object = platforms[i];
		object.move(speeds[i], 0, 0);
		float x = object.getPos().x;
		if (x > fp.tileSize.x / 2 || x < -fp.tileSize.x / 2) {
			float upperLimit;
			if (i == 0) {
				upperLimit = 0.2f;
			}
			else {
				upperLimit = abs(speeds[i - 1]);
			}
			floatCast.f = speeds[i];
			speeds[i] = generateSpeed(0.15f / (i+1), upperLimit, !floatCast.parts.sign);

			float startPoint;
			if (speeds[i] >= 0) {
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
	for (uint i = 0; i < enemies.size(); ++i) {
		if (enemies[i].isInsideViewFrustrum(frustum))
			objects[enemies[i].meshId].push_back(&enemies[i]);
	}
	for (uint i = 0; i < floorTiles.size(); ++i) {
		if (floorTiles[i].isInsideViewFrustrum(frustum))
			texturedObjects[floorTiles[i].texture].push_back(&floorTiles[i]);
	}
	for (uint i = 0; i < furniture.size(); ++i) {
		if (furniture[i].isInsideViewFrustrum(frustum))
			objects[furniture[i].meshId].push_back(&furniture[i]);
	}
	for (uint i = 0; i < platforms.size(); ++i) {
		if (platforms[i].isInsideViewFrustrum(frustum)) {
			objects[platforms[i].meshId].push_back(&platforms[i]);
		}
	}
}

vector<Jumper>* FloorRow::getEnemies() {
	return &enemies;
}

vector<CellProperties>* FloorRow::getRowObjects() {
	return &rowObjects;
}

vector<ShadowedObject>* FloorRow::getPlatforms()
{
	return &platforms;
}

bool FloorRow::isSafeZone() const {
	return safeZone;
}
BiomeType FloorRow::getBiome() const {
	return biomeType;
}

FloorRow::FloorRow() {
}


FloorRow::~FloorRow()
{
}

float FloorRow::offset;
FloorParams FloorRow::fp;
FloorResources FloorRow::res;