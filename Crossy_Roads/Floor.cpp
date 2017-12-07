#include "Floor.h"
#include <glm\glm.hpp>
using namespace glm;

Floor::Floor()
{
}

Floor::~Floor()
{
}

void Floor::firstInit() {
	tileSize = vec2(58, 2);
	rows = 40;
	cols = (uint)tileSize.x/2;
	floorRows.resize(rows);
}

inline uint between(uint min, uint max) {
	float num = ((float)rand() / RAND_MAX)*(max - min) + min;
	uint floor = (uint)num;
	if (num - floor >= 0.5)
		return floor + 1;
	else
		return floor;
}

inline void updateSafeZoneMap(uint size, uint cols, vector<vector<IdMesh>> furniture, vector<vector<IdMesh>>& map) {
	map.resize(size, vector<IdMesh>(cols, INVALID));
	vector<vec2> indices(size*cols);
	for (uint i = 0; i < size; ++i) {
		for (uint j = 0; j < cols; ++j) {
			indices[i*cols + j] = vec2(i, j);
		}
	}

	uint objects = between(size, size*2);

	for (uint i = 0; i < objects; ++i) {
		uint index = rand() % indices.size();
		vec2 pos = indices[index];
		indices.erase(indices.begin() + index);
		IdMesh object = furniture[rand() % furniture.size()][0];
		map[pos.x][pos.y] = object;
	}
}

void Floor::updateFloorRow(FloorRow& floorRow) {
	bool transition = false;
	if (counter == length) {
		switch (type) {
		case Safe:
			length = between(3, 10);
			type = Road;
			transition = true;
			break;
		case Road:
			length = between(1, 4);
			updateSafeZoneMap(length, cols, furniture, safeZoneMap);
			type = Safe;
			break;
		}
		counter = 0;
	}
	switch (type) {
	case Safe:
		floorRow.initSafeZone(safeZoneMap[safeZoneMap.size()-1]);
		safeZoneMap.pop_back();
		break;
	case Road:
		if (transition) {
			for (uint i = 0; i < cols; ++i)
				textureIndex[i] = 999;
		}
		floorRow.initRoad(textureIndex);
		break;
	}
	++counter;
}

void Floor::init(vec3 lightDir, const Assets& assets) {
	GameObject::init();

	deckMesh[0] = assets.getMeshId("cubierta_4");
	deckMesh[1] = assets.getMeshId("cubierta_1");
	deckMesh[2] = assets.getMeshId("cubierta_3");
	deckMesh[3] = assets.getMeshId("cubierta_2");

	furniture.resize(2, vector<IdMesh>(1));

	furniture[0][0] = assets.getMeshId("box");
	furniture[1][0] = assets.getMeshId("barrel");

	FloorRow::initIds(assets);
	FloorRow::setParameters(tileSize, cols, lightDir);

	float realTileSize = tileSize.x / cols;

	float offsetZ = -tileSize.y*(rows/2);
	textureIndex.resize(cols, 999);

	type = (FloorType)(rand()%2);
	length = 0;
	counter = length;
	for (uint i = 0; i < rows; ++i) {
		floorRows[i].setPos(vec2(0, offsetZ + i*tileSize.y));
		updateFloorRow(floorRows[i]);
	}
	lastRow = 0;
	firstPos = floorRows[floorRows.size()-1].getPos().y;
}

void Floor::addLevel() {
	firstPos += tileSize.y;
	vec2 lastPos = floorRows[lastRow].getPos();
	floorRows[lastRow].setPos(vec2(lastPos.x, firstPos));
	updateFloorRow(floorRows[lastRow]);

	if (lastRow == rows - 1) {
		lastRow = 0;
	}
	else {
		++lastRow;
	}
}

void Floor::update(int deltaTime) {
	for (FloorRow& row : floorRows) {
		row.update(deltaTime);
	}
}

void Floor::groupDrawableObjects(std::vector<std::vector<Object*>>& objects, std::vector<std::vector<TexturedObject*>>& texturedObjects, const FrustumG& frustum) {
	for (uint i = 0; i < rows; ++i) {
		floorRows[i].groupDrawableObjects(objects, texturedObjects, frustum);
	}
}

vec2 Floor::getTileSize() {
	return tileSize;
}

uint Floor::getRows() const {
	return rows;
}
const FloorRow* Floor::getFloorRow(uint index) const {
	return &floorRows[index];
}