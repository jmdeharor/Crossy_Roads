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

inline void updateSafeZoneMap(uint size, uint cols, vector<MeshConfig>& furniture, vector<vector<CellProperties>>& map) {
	CellProperties aux;
	aux.mesh = INVALID;
	map.resize(size, vector<CellProperties>(cols, aux));
	vector<ivec2> indices;
	indices.reserve(size*cols);

	uint objects = between(size, size*2);

	for (uint i = 0; i < objects; ++i) {

		MeshConfig& meshConfig = furniture[rand() % furniture.size()];

		for (uint i = 0; i < size-meshConfig.rows+1; ++i) {
			for (uint j = 0; j < cols-meshConfig.cols+1; ++j) {
				bool conflict = false;
				for (uint i1 = 0; i1 < meshConfig.rows && !conflict; ++i1) {
					for (uint j1 = 0; j1 < meshConfig.cols && !conflict; ++j1) {
						conflict = map[i + i1][j + j1].mesh != INVALID;
					}
				}
				if (!conflict)
					indices.push_back(ivec2(i, j));
			}
		}

		uint index = rand() % indices.size();
		ivec2 pos = indices[index];

		for (uint i = 0; i < meshConfig.rows; ++i) {
			for (uint j = 0; j < meshConfig.cols; ++j) {
				CellProperties cell;
				cell.mesh = meshConfig.meshes[i*meshConfig.cols + j];
				cell.height = meshConfig.height;
				map[pos.x+i][pos.y+j] = cell;
			}
		}
		indices.clear();
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

	furniture.resize(3);

	MeshConfig configAux;
	configAux.rows = 1;
	configAux.cols = 1;
	configAux.height = 2;
	configAux.meshes = new IdMesh;
	*configAux.meshes = assets.getMeshId("box");
	furniture[0] = configAux;

	configAux.meshes = new IdMesh;
	*configAux.meshes = assets.getMeshId("barrel");
	furniture[1] = configAux;

	configAux.rows = 2;
	configAux.cols = 2;
	configAux.height = 0.5f;
	configAux.meshes = new IdMesh[4];
	configAux.meshes[0] = assets.getMeshId("cubierta_4");
	configAux.meshes[1] = assets.getMeshId("cubierta_1");
	configAux.meshes[2] = assets.getMeshId("cubierta_3");
	configAux.meshes[3] = assets.getMeshId("cubierta_2");
	furniture[2] = configAux;
	configAux.meshes = NULL;

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