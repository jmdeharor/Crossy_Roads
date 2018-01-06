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
	//colOffset = 1;
	colOffset = 2;
	rowOffset = 4;
	//tileSize = vec2(54, 2);
	//rows = 22;
	rows = 30;
	cols = 31;
	float singleTileSize = 2;
	tileSize = vec2(cols*singleTileSize, singleTileSize);
	floorRows.resize(rows);
	playableLowerLimit = 6; //Right
	playableUpperLimit = 20; //Left
}

inline void setMapObstacle(ivec2 pos, const MeshConfig& meshConfig, vector<vector<CellProperties>>& map) {
	CellProperties cell;
	cell.occupied = true;
	cell.empty = meshConfig.floorEmpty;
	cell.height = meshConfig.height;
	for (uint i = 0; i < meshConfig.rows; ++i) {
		for (uint j = 0; j < meshConfig.cols; ++j) {
			cell.collision = meshConfig.collisionMap[i*meshConfig.cols + j];
			map[pos.x + i][pos.y + j] = cell;
		}
	}
	map[pos.x][pos.y].mesh = meshConfig.mesh;
	map[pos.x][pos.y].rows = meshConfig.rows;
	map[pos.x][pos.y].cols = meshConfig.cols;
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
		aux.occupied = true;
		aux.collision = true;
		for (uint j = 0; j < cols; ++j) {
			MeshConfig config = railMesh->getMeshConfig();
			aux.height = config.height;
			aux.rows = config.rows;
			aux.cols = config.cols;
			aux.mesh = config.mesh;
			map[plankLength][j] = aux;
		}

		uint plankPos = between((int)playableLowerLimit+2, playableUpperLimit-2);
		map[plankLength][plankPos].mesh = INVALID;
		map[plankLength][plankPos].height = 0;
		map[plankLength][plankPos].occupied = true;
		map[plankLength][plankPos].collision = false;

		aux.height = 0.1f;
		aux.verticalOffset = 5;
		aux.collision = false;
		aux.occupied = true;
		aux.canJump = true;
		aux.mesh = INVALID;
		for (uint i = 0; i < plankLength; ++i) {
			map[i][plankPos] = aux;
		}
		map[0][plankPos].rows = plankLength;
		map[0][plankPos].cols = 1;
		map[0][plankPos].mesh = plankMesh;
		start = plankLength+1;
		if (start < map.size()) {
			map[start][plankPos].occupied = true;
		}
	}
	else
		start = 0;

	static vector<uint> decoration;
	uint furnitureIndex = sub2ind(biome, MeshConfigGroup::Border);
	decoration.reserve(furniture[furnitureIndex].size());
	uint i = start;
	while (i < size) {
		for (uint j = 0; j < furniture[furnitureIndex].size(); ++j) {
			if (i + furniture[furnitureIndex][j]->getRows() <= size)
				decoration.push_back(j);
		}
		MeshConfig meshConfig = furniture[furnitureIndex][decoration[rand()%decoration.size()]]->getMeshConfig();
		ivec2 pos = ivec2(i, playableLowerLimit - meshConfig.cols);
		CellProperties cell;
		cell.occupied = true;
		cell.empty = meshConfig.floorEmpty;
		cell.height = meshConfig.height;
		cell.collision = true;
		for (uint i = 0; i < meshConfig.rows; ++i) {
			for (uint j = 0; j < meshConfig.cols; ++j) {
				map[pos.x + i][pos.y + j] = cell;
			}
		}
		map[pos.x][pos.y].mesh = meshConfig.mesh;
		map[pos.x][pos.y].rows = meshConfig.rows;
		map[pos.x][pos.y].cols = meshConfig.cols;
		i += meshConfig.rows;
		decoration.clear();
	}
	i = start;
	while (i < size) {
		for (uint j = 0; j < furniture[furnitureIndex].size(); ++j) {
			if (i + furniture[furnitureIndex][j]->getRows() <= size)
				decoration.push_back(j);
		}
		MeshConfig meshConfig = furniture[furnitureIndex][decoration[rand() % decoration.size()]]->getMeshConfig();
		ivec2 pos = ivec2(i, playableUpperLimit);
		CellProperties cell;
		cell.occupied = true;
		cell.empty = meshConfig.floorEmpty;
		cell.height = meshConfig.height;
		cell.collision = true;
		for (uint i = 0; i < meshConfig.rows; ++i) {
			for (uint j = 0; j < meshConfig.cols; ++j) {
				map[pos.x + i][pos.y + j] = cell;
			}
		}
		map[pos.x][pos.y].mesh = meshConfig.mesh;
		map[pos.x][pos.y].rows = meshConfig.rows;
		map[pos.x][pos.y].cols = meshConfig.cols;
		i += meshConfig.rows;
		decoration.clear();
	}

	vector<ivec2> indices;
	indices.reserve(size*cols);

	uint objects = between((int)size, size*2);

	furnitureIndex = sub2ind(biome, MeshConfigGroup::Decoration);
	for (uint i = 0; i < objects; ++i) {
		MeshConfig meshConfig = furniture[furnitureIndex][rand() % furniture[furnitureIndex].size()]->getMeshConfig();

		for (int i = start; i < (int)size-(int)meshConfig.rows+1; ++i) {
			for (int j = playableLowerLimit; j <= (int)playableUpperLimit-(int)meshConfig.cols+1; ++j) {
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
		setMapObstacle(pos, meshConfig, map);
		indices.clear();
	}
}

void Floor::updateFloorRow(FloorRow& floorRow, const FloorRow& prevRow) {
	if (biomeCounter == biomeLength) {
		float randomF;
		static const float shipProb = 0.5f;
		
		switch (biome) {
		case Ship:
			biomeLength = between(10, 30);
			biome = Sea;
			break;
		case Sea:
			biomeLength = between(20, 40);
			type = Road;
			counter = length = 0;
			
			randomF = rand() / (float)RAND_MAX;

			if (randomF < shipProb)
				biome = Ship;
			else
				biome = Island;
			break;
		case Island:
			biomeLength = between(10, 30);
			type = Road;
			counter = length = 0;
			biome = Sea;
			break;
		}
		biomeCounter = 0;
	}
	vector<uint> aux;
	vector<CellProperties> aux2;
	switch (biome) {
	case Sea:
		if (map.size() > 0) {
			floorRow.initRoad(biome, aux, map[map.size() - 1], prevRow);
			map.pop_back();
		}
		else {
			floorRow.initRoad(biome, aux, aux2, prevRow);
		}
		break;
	case Island:
		if (counter == length) {
			switch (type) {
			case Safe:
				length = between(3, 10);
				if (length >= biomeLength - biomeCounter) {
					length = biomeLength - biomeCounter - 1;
				}
				type = Road;
				break;
			case Road:
				length = between(1, 4);
				if (length >= biomeLength - biomeCounter) {
					length = biomeLength - biomeCounter;
				}
				else if (length + 1 == biomeLength - biomeCounter) {
					length = biomeLength - biomeCounter;
				}
				updateMap(false, length);
				type = Safe;
				break;
			}
			counter = 0;
		}
		switch (type) {
		case Safe:
			floorRow.initSafeZone(biome, map[map.size() - 1], prevRow);
			map.pop_back();
			break;
		case Road:
			floorRow.initRoad(biome, textureIndex, {}, prevRow);
			break;
		}
		++counter;
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
			floorRow.initSafeZone(biome, map[map.size() - 1], prevRow);
			map.pop_back();
			break;
		case Road:
			if (transition) {
				for (uint i = 0; i < cols; ++i)
					textureIndex[i] = 999;
			}
			floorRow.initRoad(biome, textureIndex, {}, prevRow);
			break;
		}
		if (biomeCounter == 0)
			floorRow.putWall();
		++counter;
		break;
	}
	++biomeCounter;
}

void Floor::init(vec3 lightDir, const Assets& assets, const Player* player) {
	GameObject::init();

	furniture = assets.getMeshConfigGroups();

	plankMesh = assets.getMeshId("plank");
	railMesh = (const RandomMeshConfig*)assets.getMeshConfigId("railing");

	FloorRow::initResources(assets, player);
	FloorParams params;
	params.tileSize = tileSize;
	params.colOffset = colOffset;
	params.lightDir = lightDir;
	params.realTileSize = tileSize.x / cols;
	params.lowerLimit = -tileSize.x / 2 + colOffset*params.realTileSize;
	params.upperLimit = tileSize.x / 2 + colOffset*params.realTileSize;
	params.offsetX = - (params.realTileSize*(cols / 2) - (1 - cols % 2)*params.realTileSize / 2);
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
	floorRows[rows - 1].initAttributes(Ship, false, 0);

	ivec2 playerIni;
	playerIni.x = length - 1 - (rows / 2 - rowOffset);
	playerIni.y = cols / 2 - colOffset;

	updateMap(false, length, { playerIni });
	for (uint i = 0; i < rows; ++i) {
		floorRows[i].firstInit();
		floorRows[i].pos = vec2(colOffset*realTileSize, rowOffset*tileSize.y + offsetZ + i*tileSize.y);
		updateFloorRow(floorRows[i], floorRows[i > 0 ? i-1 : rows-1]);
	}
	lastRow = 0;
	firstPos = floorRows[floorRows.size()-1].pos.y;
}

void Floor::addLevel() {
	firstPos += tileSize.y;
	vec2 lastPos = floorRows[lastRow].pos;
	floorRows[lastRow].pos = vec2(lastPos.x, firstPos);
	uint prevRowI = (int)lastRow - 1 < 0 ? rows - 1 : lastRow - 1;
	updateFloorRow(floorRows[lastRow], floorRows[prevRowI]);

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