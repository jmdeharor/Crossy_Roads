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

inline void updateSafeZoneMap(uint size, uint cols, const vector<MeshConfig>& furniture, vector<vector<CellProperties>>& map, ivec2 restriction) {
	CellProperties aux;
	aux.height = aux.verticalOffset = 0;
	aux.mesh = INVALID;
	aux.occupied = false;
	aux.collision = false;
	map.resize(size, vector<CellProperties>(cols, aux));
	vector<ivec2> indices;
	indices.reserve(size*cols);

	uint objects = between(size, size * 2);

	for (uint i = 0; i < objects; ++i) {

		const MeshConfig& meshConfig = furniture[rand() % furniture.size()];

		for (int i = 0; i < (int)size - (int)meshConfig.rows + 1; ++i) {
			bool rowConflict = i <= restriction.x && i + (int)meshConfig.rows >= restriction.x;
			for (int j = 0; j < (int)cols - (int)meshConfig.cols + 1; ++j) {
				bool colConflict = j <= restriction.y && j + (int)meshConfig.cols >= restriction.y;
				bool conflict = rowConflict && colConflict;
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

		for (uint i = 0; i < meshConfig.rows; ++i) {
			for (uint j = 0; j < meshConfig.cols; ++j) {
				CellProperties cell;
				cell.mesh = INVALID;
				cell.collision = true;
				cell.occupied = true;
				cell.verticalOffset = 0;
				cell.height = meshConfig.height;
				map[pos.x + i][pos.y + j] = cell;
			}
		}
		map[pos.x][pos.y].mesh = meshConfig.mesh;
		map[pos.x][pos.y].rows = meshConfig.rows;
		map[pos.x][pos.y].cols = meshConfig.cols;
		indices.clear();
	}
}

const uint plankLength = 3;

void Floor::updateMap(bool lastRow, uint size) {
	CellProperties aux;
	aux.height = aux.verticalOffset = 0;
	aux.mesh = INVALID;
	aux.occupied = false;
	aux.collision = false;

	uint prevSize = map.size();
	map.resize(size, vector<CellProperties>(cols, aux));
	for (uint i = 0; i < std::min(prevSize, size); ++i) {
		for (uint j = 0; j < cols; ++j) {
			map[i][j] = aux;
		}
	}

	uint start;

	if (lastRow) {
		aux.verticalOffset = 0;
		aux.height = 1.5f;
		aux.mesh = railMesh;
		aux.occupied = true;
		aux.collision = true;
		aux.cols = 1;
		aux.rows = 1;
		for (uint j = 0; j < cols; ++j) {
			map[plankLength][j] = aux;
		}

		uint plankPos = between(cols / 2 - 3, cols / 2 + 3);
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

	uint objects = between(size, size*2);

	for (uint i = 0; i < objects; ++i) {
		const MeshConfig& meshConfig = (*furniture)[rand() % furniture->size()];

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

		for (uint i = 0; i < meshConfig.rows; ++i) {
			for (uint j = 0; j < meshConfig.cols; ++j) {
				CellProperties cell;
				cell.mesh = INVALID;
				cell.verticalOffset = 0;
				cell.collision = true;
				cell.occupied = true;
				cell.height = meshConfig.height;
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
		biomeLength = between(15, 20);
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
	railMesh = assets.getMeshId("railing_parrot");

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
	biomeLength = between(15, 20);
	biomeCounter = 0;

	type = Safe;
	length = rows/2 - rowOffset + between(2, 5);
	counter = 0;

	ivec2 playerIni;
	playerIni.x = length - 1 - (rows / 2 - rowOffset);
	playerIni.y = cols / 2 - colOffset;

	updateSafeZoneMap(length, cols, *furniture, map, playerIni);
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

glm::uint Floor::getRowOffset() const
{
	return rowOffset;
}

uint Floor::getRows() const {
	return rows;
}
glm::uint Floor::getCols() const {
	return cols;
}
FloorRow* Floor::getFloorRow(uint index) {
	return &floorRows[index];
}