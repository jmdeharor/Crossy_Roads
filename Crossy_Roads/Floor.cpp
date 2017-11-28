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
	cubeMesh.init();
	tileSize = vec2(60, 2);
	rows = 40;
	cols = (uint)tileSize.x/2;
	floorRows.resize(rows);
	FloorRow::initMeshes();

	deckMesh[0].loadFromFile("models/cubierta_4.obj");
	deckMesh[1].loadFromFile("models/cubierta_1.obj");
	deckMesh[2].loadFromFile("models/cubierta_3.obj");
	deckMesh[3].loadFromFile("models/cubierta_2.obj");
}

inline uint between(uint min, uint max) {
	float num = ((float)rand() / RAND_MAX)*(max - min) + min;
	uint floor = (uint)num;
	if (num - floor >= 0.5)
		return floor + 1;
	else
		return floor;
}

void Floor::init(vec3 lightDir) {
	GameObject::init();

	FloorRow::setParameters(tileSize, cols, lightDir);

	float realTileSize = tileSize.x / cols;

	float offsetZ = -tileSize.y*(rows/2);
	vector<uint> meshIndex(cols, 999);
	vector<vector<Mesh*>> map;

	FloorType type = (FloorType)(rand()%2);
	uint length = 0;
	uint counter = length;
	for (uint i = 0; i < rows; ++i) {
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
				map.resize(length);

				for (uint i = 0; i < length; ++i) {
					map[i].resize(cols);
					for (uint j = 0; j < cols; ++j) {
						map[i][j] = &cubeMesh;
					}
				}

				if (length >= 2) {
					uint start = between(0, cols - 2);
					for (uint i = 0; i < 2; ++i) {
						for (uint j = 0; j < 2; ++j) {
							map[i][start + j] = &deckMesh[i * 2 + j];
						}
					}
				}

				type = Safe;
				break;
			}
			counter = 0;
		}
		floorRows[i].setPos(vec2(0, offsetZ + i*tileSize.y));
		switch (type) {
		case Safe:
			floorRows[i].initSafeZone(map[map.size()-1]);
			map.pop_back();
			break;
		case Road:
			if (transition) {
				for (uint i = 0; i < cols; ++i)
					meshIndex[i] = 999;
			}
			floorRows[i].initRoad(meshIndex);
			break;
		}
		++counter;
	}
	lastRow = 0;
	firstPos = floorRows[floorRows.size()-1].getPos().y;
}

void Floor::addLevel() {
	firstPos += tileSize.y;
	vec2 lastPos = floorRows[lastRow].getPos();
	floorRows[lastRow].moveToPosition(vec2(lastPos.x, firstPos));
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

void Floor::renderSimpleObjects(ShaderProgram & program) {
	for (FloorRow& row : floorRows) {
		row.renderSimpleObjects(program);
	}
}

void Floor::renderLightObjects(ShaderProgram & program) {
	for (FloorRow& row : floorRows) {
		row.renderLightObjects(program);
	}
}

void Floor::renderShadows(ShaderProgram & program) {
	for (FloorRow& row : floorRows) {
		row.renderShadows(program);
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