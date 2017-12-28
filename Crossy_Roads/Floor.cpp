#include "Floor.h"
#include <cmath>
#include <glm\glm.hpp>
#include "Utils.h"
using namespace glm;

Floor::Floor()
{
}

Floor::~Floor()
{
}

void Floor::firstInit() {
	colOffset = 1;
	rowOffset = 4;
	tileSize = vec2(54, 2);
	rows = 22;
	cols = (uint)tileSize.x/(uint)tileSize.y;
	floorRows.resize(rows);
}

const uint plankLength = 3;

void Floor::updateMap(bool lastRow, uint size, const vector<ivec2>& restrictions) {
	CellProperties aux;

	uint prevSize = map.size();
	map.resize(size, vector<CellProperties>(cols, aux));
	for (uint i = 0; i < std::min(prevSize, size); ++i) {
		for (uint j = 0; j < cols; ++j) {
			map[i][j] = aux;
		}
	}

	for (const ivec2& restriction : restrictions) {
		map[restriction.x][restriction.y].occupied = true;
	}

	uint start;

	if (lastRow) {
		aux.height = 1.5f;
		aux.occupied = true;
		aux.collision = true;
		aux.cols = 1;
		aux.rows = 1;
		for (uint j = 0; j < cols; ++j) {
			aux.mesh = railMesh->getMesh();
			map[plankLength][j] = aux;
		}

		uint plankPos = between((int)cols / 2 - 3, cols / 2 + 3);
		map[plankLength][plankPos].mesh = INVALID;
		map[plankLength][plankPos].height = 0;
		map[plankLength][plankPos].occupied = true;
		map[plankLength][plankPos].collision = false;

		aux.height = 0.1f;
		aux.verticalOffset = 5;
		aux.collision = false;
		aux.occupied = true;
		aux.mesh = INVALID;
		for (uint i = 0; i < plankLength; ++i) {
			map[i][plankPos] = aux;
		}
		map[0][plankPos].rows = plankLength;
		map[0][plankPos].cols = 1;
		map[0][plankPos].mesh = plankMesh;
		start = plankLength;
		if (plankLength + 1 < map.size()) {
			map[plankLength + 1][plankPos].occupied = true;
		}
	}
	else
		start = 0;

	vector<ivec2> indices;
	indices.reserve(size*cols);

	uint objects = between((int)size, size*2);

	for (uint i = 0; i < objects; ++i) {
		const MeshConfig& meshConfig = furniture[biome][rand() % furniture[biome].size()];

		for (int i = start; i < (int)size-(int)meshConfig.rows+1; ++i) {
			for (int j = start; j < (int)cols-(int)meshConfig.cols+1; ++j) {
				bool conflict = false;
				for (uint i1 = 0; i1 < meshConfig.rows && !conflict; ++i1) {
					for (uint j1 = 0; j1 < meshConfig.cols && !conflict; ++j1) {
						conflict = map[i + i1][j + j1].occupied;
					}
				}
				if (!conflict)
					indices.push_back(ivec2(i, j));
			}
		}
		if (indices.size() == 0)
			continue;

		uint index = rand() % indices.size();
		ivec2 pos = indices[index];
		CellProperties cell;
		cell.collision = true;
		cell.occupied = true;
		cell.empty = meshConfig.floorEmpty;
		cell.height = meshConfig.height;
		for (uint i = 0; i < meshConfig.rows; ++i) {
			for (uint j = 0; j < meshConfig.cols; ++j) {
				map[pos.x+i][pos.y+j] = cell;
			}
		}
		map[pos.x][pos.y].mesh = meshConfig.mesh;
		map[pos.x][pos.y].rows = meshConfig.rows;
		map[pos.x][pos.y].cols = meshConfig.cols;
		indices.clear();
	}
}

void Floor::updateFloorRow(FloorRow& floorRow) {
	if (biomeCounter == biomeLength) {
		switch (biome) {
		case Ship:
			biome = Sea;
			break;
		case Sea:
			type = Road;
			counter = length = 0;
			biome = Ship;
			break;
		}
		biomeLength = between(20, 40);
		biomeCounter = 0;
	}
	vector<uint> aux;
	vector<CellProperties> aux2;
	switch (biome) {
	case Sea:
		if (map.size() > 0) {
			floorRow.initRoad(Sea, aux, map[map.size() - 1]);
			map.pop_back();
		}
		else {
			floorRow.initRoad(Sea, aux, aux2);
		}
		break;
	case Ship:
		bool transition = false;
		if (counter == length) {
			switch (type) {
			case Safe:
				length = between(3, 10);
				if (length >= biomeLength - biomeCounter) {
					length = biomeLength - biomeCounter-1;
				}
				type = Road;
				transition = true;
				break;
			case Road:
				length = between(1, 4);
				bool lastRow;
				if (length >= biomeLength - biomeCounter) {
					length = biomeLength - biomeCounter + plankLength;
					lastRow = true;
				}
				else if (length + 1 == biomeLength - biomeCounter) {
					lastRow = true;
					length = biomeLength - biomeCounter + plankLength;
				}
				else
					lastRow = false;
				updateMap(lastRow, length);
				type = Safe;
				break;
			}
			counter = 0;
		}
		switch (type) {
		case Safe:
			floorRow.initSafeZone(biome, map[map.size() - 1]);
			map.pop_back();
			break;
		case Road:
			if (transition) {
				for (uint i = 0; i < cols; ++i)
					textureIndex[i] = 999;
			}
			floorRow.initRoad(biome, textureIndex, {});
			break;
		}
		++counter;
		break;
	}
	++biomeCounter;
}

void Floor::init(vec3 lightDir, const Assets& assets) {
	GameObject::init();

	furniture = assets.getDecoration();

	plankMesh = assets.getMeshId("plank");
	railMesh = assets.getRandomMesh("railing");

	FloorRow::initResources(assets);
	FloorParams params;
	params.tileSize = tileSize;
	params.colOffset = colOffset;
	params.lightDir = lightDir;
	params.realTileSize = tileSize.x / cols;
	params.cols = cols;
	FloorRow::setParameters(params);

	float realTileSize = tileSize.x / cols;

	float offsetZ = -tileSize.y*(rows/2);
	textureIndex.resize(cols, 999);

	biome = Ship;
	biomeLength = between(20, 40);
	biomeCounter = 0;

	type = Safe;
	length = rows/2 - rowOffset + between(2, 5);
	counter = 0;

	ivec2 playerIni;
	playerIni.x = length - 1 - (rows / 2 - rowOffset);
	playerIni.y = cols / 2 - colOffset;

	updateMap(false, length, { playerIni });
	for (uint i = 0; i < rows; ++i) {
		floorRows[i].pos = vec2(colOffset*realTileSize, rowOffset*tileSize.y + offsetZ + i*tileSize.y);
		updateFloorRow(floorRows[i]);
	}
	lastRow = 0;
	firstPos = floorRows[floorRows.size()-1].pos.y;
}

void Floor::addLevel() {
	firstPos += tileSize.y;
	vec2 lastPos = floorRows[lastRow].pos;
	floorRows[lastRow].pos = vec2(lastPos.x, firstPos);
	updateFloorRow(floorRows[lastRow]);

	lastRow = (lastRow + 1) % rows;
}

void Floor::update(int deltaTime) {
	for (FloorRow& row : floorRows) {
		row.update(deltaTime);
	}
}

void Floor::groupDrawableObjects(const FrustumG& frustum, RenderVectors& renderVectors) {
	for (uint i = 0; i < rows; ++i) {
		floorRows[i].groupDrawableObjects(frustum, renderVectors);
	}
}

vec2 Floor::getTileSize() const {
	return tileSize;
}

uint Floor::getColOffset() const {
	return colOffset;
}

uint Floor::getRowOffset() const {
	return rowOffset;
}

uint Floor::getRows() const {
	return rows;
}

uint Floor::getCols() const {
	return cols;
}
FloorRow* Floor::getFloorRow(uint index) {
	return &floorRows[index];
}