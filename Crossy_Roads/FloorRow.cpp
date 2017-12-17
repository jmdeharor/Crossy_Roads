#include "FloorRow.h"
#include "Scene.h"
#include "Pi.h"
using namespace glm;

const vector<vector<string>> models = { 
	{ "wood_3_1", "wood_3_2", "wood_3_3", "wood_3_4", "wood_3_5" },
	{ "wood_4_0", "wood_4_1", "wood_4_2", "wood_4_3", "wood_4_4" },
	{ "wood_5_0", "wood_5_1", "wood_5_2", "wood_5_3", "wood_5_4" },
	{ "wood_6_0", "wood_6_1", "wood_6_2", "wood_6_3", "wood_6_4" },
	{ "wood_7_0", "wood_7_1", "wood_7_2", "wood_7_3", "wood_7_4" } };

const vector<string> enemyMeshesStrings = {
	"pirate", "pirate_2"
};

union {
	float f;
	struct {
		unsigned int mantisa : 23;
		unsigned int exponent : 8;
		unsigned int sign : 1;
	} parts;
} floatCast;

vector<uint> indices;

void FloorRow::initIds(const Assets& assets) {
	FloorRow::assets = &assets;
	enemyMeshes.resize(enemyMeshesStrings.size());
	for (uint i = 0; i < enemyMeshesStrings.size(); ++i) {
		enemyMeshes[i] = assets.getMeshId(enemyMeshesStrings[i]);
	}

	cubeMesh = assets.getCubeMesh();
	indices.resize(models.size());
	for (uint i = 0; i < indices.size(); ++i) {
		indices[i] = i;
	}

	floorTextures.resize(models.size());
	for (uint i = 0; i < models.size(); ++i) {
		floorTextures[i].resize(models[i].size());
		for (uint j = 0; j < models[i].size(); ++j) {
			floorTextures[i][j] = assets.getTextureId(models[i][j]);
		}
	}
	planeWood = assets.getTextureId("wood_plane");
	water = assets.getTextureId("water_3");
	crocodile = assets.getMeshId("crocodile-01");
	shark = assets.getMeshId("shark-00");
}

void FloorRow::setParameters(vec2 tileSize, uint cols, vec3 lightDir, uint colOffset) {
	FloorRow::tileSize = tileSize;
	FloorRow::cols = cols;
	FloorRow::lightDir = lightDir;
	realTileSize = tileSize.x / cols;
	offset = -tileSize.x/2 + colOffset*realTileSize;
}

float FloorRow::worldToCol(float x)
{
	return (x - offset) / realTileSize;
}

vec2 FloorRow::getPos() const {
	return pos;
}

const float maxSpeed = 0.3f;
const float minSpeed = 0.05f;

inline float generateSpeed() {
	return (((float)rand() / RAND_MAX)*(maxSpeed - minSpeed) + minSpeed)*(-1 + (rand()%2)*2);
}

inline float generateSpeed(float minSpeed, float maxSpeed, bool sign) {
	return (((float)rand() / RAND_MAX)*(maxSpeed - minSpeed) + minSpeed)*(-1 + sign*2);
}

void FloorRow::setPos(vec2 position) {
	pos = position;
}

void FloorRow::initRoad(Biome biome, vector<uint>* adjacentRow, vector<CellProperties>* map) {
	this->biome = biome;
	safeZone = false;
	switch (biome) {
		case Ship:
			initShipRoad(*adjacentRow);
			break;
		case Sea:
			initSea(map);
			break;
	}
}

bool applyConstraints(uint meshIndex, uint numAdjacents, vector<uint>& adjacentRow, uint position) {
	if (position + numAdjacents < adjacentRow.size() && adjacentRow[position + numAdjacents - 1] != adjacentRow[position + numAdjacents])
		return false;
	if (meshIndex == adjacentRow[position])
		return false;
	return true;
}

inline int between(int min, int max) {
	float num = ((float)rand() / RAND_MAX)*(max - min) + min;
	return (int)round(num);
}

pair<uint,uint> generateRandomTextureIndex(uint i, uint prevMeshIndex, vector<uint>& adjacentRow) {
	uint textureIndex;
	uint numAdjacentTiles;
	vector<uint> indicesCopy = indices;
	while (indicesCopy.size() > 0) {
		uint ind = rand() % indicesCopy.size();
		textureIndex = indicesCopy[ind];
		indicesCopy.erase(indicesCopy.begin() + ind);
		if (prevMeshIndex == textureIndex)
			continue;
		for (numAdjacentTiles = models[textureIndex].size(); numAdjacentTiles >= 4; --numAdjacentTiles) {
			if (applyConstraints(textureIndex, numAdjacentTiles, adjacentRow, i)) {
				indicesCopy.clear();
				break;
			}
		}
	}
	return make_pair(textureIndex, numAdjacentTiles);
}

void FloorRow::initSafeZone(vector<CellProperties>& map) {
	biome = Ship;
	safeZone = true;
	platforms.clear();
	enemies.clear();
	speeds.clear();
	rowObjects = map;
	floorTiles.resize(1);
	rowHeight = 0.2f;

	static float realTileSize = tileSize.x / cols;
	float offsetX = pos.x - (realTileSize*(cols / 2) - (1 - cols % 2)*realTileSize / 2);

	static vec3 boundingBox = cubeMesh->getbbSize();
	static vec3 bbcenter = cubeMesh->getbbCenter();
	static float height = cubeMesh->getHeight();
	static vec3 floorTileSize = vec3(tileSize.x, 0.2f, tileSize.y) / boundingBox;

	TexturedObject& tile = floorTiles[0];
	tile.texture = planeWood;
	tile.setMesh(cubeMesh);
	tile.setScale(floorTileSize);
	tile.setCenter(vec3(bbcenter.x, bbcenter.y + height / 2.f, bbcenter.z));
	tile.setPos(vec3(pos.x, rowHeight, pos.y));

	furniture.clear();
	for (uint i = 0; i < cols; ++i) {
		if (map[i].height == 0 || map[i].mesh == INVALID)
			continue;
		furniture.push_back(ShadowedObject());
		ShadowedObject& object = furniture[furniture.size()-1];
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
		object.setPos(vec3(posX + (realTileSize*(map[i].cols/2.f)) - realTileSize/2, rowHeight, pos.y - tileSize.y*(map[i].rows / 2.f) + tileSize.y/2));
		object.setPlane(vec4(0, 1, 0, -rowHeight), lightDir);
	}
}

pair<vec3, float> FloorRow::getHeight(uint col) {
	float offsetX = pos.x - (realTileSize*(cols / 2) - (1 - cols % 2)*realTileSize / 2);
	vec3 myHeight = vec3(offsetX + col*realTileSize, rowHeight, pos.y);
	float platformSpeed = 0;
	for (uint i = 0; i < platforms.size(); ++i) {
		uint index = worldToCol(platforms[i].getPos().x);
		if (index == col || index == col + 1 || index == col - 1) {
			myHeight.y = platforms[i].getPos().y + platforms[i].getHeight();
			myHeight.x = platforms[i].getPos().x + speeds[i] * 11;
			if (index == col + 1) myHeight.x -= realTileSize;
			else if (index == col - 1) myHeight.x += realTileSize;
			platformSpeed = speeds[i];
			break;
		}
	}
	return make_pair(myHeight, platformSpeed);
}

void FloorRow::initShipRoad(vector<uint>& adjacentRow) {
	rowObjects.clear();
	furniture.clear();
	platforms.clear();
	enemies.resize(2);
	speeds.resize(enemies.size());
	floorTiles.resize(cols);
	rowHeight = 0;

	static float realTileSize = tileSize.x / cols;
	static vec3 boundingBox = cubeMesh->getbbSize();
	static vec3 bbcenter = cubeMesh->getbbCenter();
	static float height = cubeMesh->getHeight();
	vec3 floorTileSize = vec3(realTileSize, 0.1f, tileSize.y) / boundingBox;
	float offsetX = pos.x - (realTileSize*(cols / 2) - (1 - cols % 2)*realTileSize / 2);

	for (uint i = 0; i < enemies.size(); ++i) {
		Jumper& enemy = enemies[i];
		enemy.setMesh(enemyMeshes[i], assets->getMesh(enemyMeshes[i]));
		enemy.setScale(vec3(0.1f));
		enemy.setCenterToBaseCenter();
		enemy.setPlane(vec4(0, 1, 0, -rowHeight), lightDir);
		enemy.horizontalSpeed = generateSpeed();
		float startPoint;
		if (enemy.horizontalSpeed >= 0) {
			enemy.setRotationY(PI / 2);
			startPoint = -tileSize.x / 2 + ((float)rand() / RAND_MAX) * tileSize.x;
		}
		else {
			enemy.setRotationY(-PI / 2);
			startPoint = tileSize.x / 2 - ((float)rand() / RAND_MAX) * tileSize.x;
		}
		enemy.setPos(vec3(startPoint, rowHeight, pos.y));
	}

	uint textureIndex = models.size();
	IdTex texture = INVALID;
	uint numAdjacentTiles = 0;
	uint counter = numAdjacentTiles;
	vector<uint> indicesAux;
	for (uint i = 0; i < floorTiles.size(); ++i) {
		TexturedObject& tile = floorTiles[i];
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

void FloorRow::initSea(vector<CellProperties>* map) {
	furniture.clear();
	enemies.clear();
	platforms.resize(between(2, 4));
	speeds.resize(platforms.size());
	rowObjects.clear();
	floorTiles.resize(cols);
	rowHeight = -5;

	static float realTileSize = tileSize.x / cols;
	float offsetX = pos.x - (realTileSize*(cols / 2) - (1 - cols % 2)*realTileSize / 2);

	static vec3 boundingBox = cubeMesh->getbbSize();
	static vec3 bbcenter = cubeMesh->getbbCenter();
	static float height = cubeMesh->getHeight();
	static vec3 floorTileSize = vec3(realTileSize, 0.2f, tileSize.y) / boundingBox;

	for (uint i = 0; i < floorTiles.size(); ++i) {
		TexturedObject& tile = floorTiles[i];
		tile.texture = water;
		tile.setRotationY(PI/2);
		tile.setMesh(cubeMesh);
		tile.setScale(floorTileSize);
		tile.setCenter(vec3(bbcenter.x, bbcenter.y + height / 2.f, bbcenter.z));
		tile.setPos(vec3(offsetX + i*realTileSize, rowHeight, pos.y));
	}

	static const Mesh* crocoMesh = assets->getMesh(crocodile);
	static const Mesh* sharkMesh = assets->getMesh(shark);
	static vec3 crocodilebb = crocoMesh->getbbSize();
	static vec3 crocoSize = vec3(3 * realTileSize, 1, tileSize.y) / crocodilebb;

	uint nPlat = platforms.size();
	for (uint i = 0; i < nPlat; ++i) {
		ShadowedObject& platform = platforms[i];
		platform.setMesh(shark, sharkMesh);
		platform.setScale(crocoSize);
		platform.setCenterToBaseCenter();
		platform.setPlane(vec4(0, 1, 0, -rowHeight), lightDir);
		float upperLimit;
		if (i == 0) {
			upperLimit = 0.2f;
		}
		else {
			upperLimit = abs(speeds[i - 1]);
		}
		speeds[i] = generateSpeed(0.1f / (i+1), upperLimit, int(pos.y / tileSize.y) % 2);
		float startPoint;
		if (speeds[i] >= 0) {
			platform.setRotationY(0);
			startPoint = -tileSize.x / 2;
		}
		else {
			platform.setRotationY(PI);
			startPoint = tileSize.x / 2;
		}
		platform.setPos(vec3(startPoint, rowHeight-0.5f, pos.y));
	}

	if (map == NULL)
		return;
	vector<CellProperties>& mapp = *map;
	for (uint i = 0; i < cols; ++i) {
		if (mapp[i].height == 0 || mapp[i].mesh == INVALID)
			continue;
		furniture.push_back(ShadowedObject());
		ShadowedObject& object = furniture[furniture.size() - 1];
		IdMesh meshId = mapp[i].mesh;
		float height = mapp[i].height;

		const Mesh* mesh = assets->getMesh(meshId);

		vec3 boundingBox = mesh->getbbSize();
		vec3 bbCenter = mesh->getbbCenter();
		vec3 objectSize = vec3(realTileSize*mapp[i].cols, height, tileSize.y*mapp[i].rows) / boundingBox;

		object.setMesh(meshId, mesh);
		object.setScale(objectSize);
		object.setCenterToBaseCenter();
		float posX = offsetX + i*realTileSize;
		object.setPos(vec3(posX + (realTileSize*(mapp[i].cols / 2.f)) - realTileSize / 2, rowHeight, pos.y - tileSize.y*(mapp[i].rows / 2.f) + tileSize.y / 2));
		object.setPlane(vec4(0, 1, 0, -rowHeight), lightDir);
	}
}

void FloorRow::update(int deltaTime) {
	for (uint i = 0; i < enemies.size(); ++i) {
		Jumper& object = enemies[i];
		object.update();
		float x = object.getPos().x;
		if (x > tileSize.x / 2 || x < -tileSize.x/2) {
			object.horizontalSpeed = generateSpeed();
			float startPoint;
			if (object.horizontalSpeed >= 0) {
				object.setRotationY(PI/2);
				startPoint = -tileSize.x / 2;
			}
			else {
				object.setRotationY(-PI/2);
				startPoint = tileSize.x / 2;
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
		if (x > tileSize.x / 2 || x < -tileSize.x / 2) {
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
				startPoint = -tileSize.x / 2;
			}
			else {
				object.setRotationY(PI);
				startPoint = tileSize.x / 2;
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

std::vector<ShadowedObject>* FloorRow::getPlatforms()
{
	return &platforms;
}

bool FloorRow::isSafeZone() const {
	return safeZone;
}
Biome FloorRow::getBiome() const {
	return biome;
}
FloorRow::FloorRow()
{
}


FloorRow::~FloorRow()
{
}

float FloorRow::offset;
float FloorRow::realTileSize;
vec2 FloorRow::tileSize;
uint FloorRow::cols;
vec3 FloorRow::lightDir;
IdMesh FloorRow::crocodile;
IdMesh FloorRow::shark;
const Mesh* FloorRow::cubeMesh;
vector<vector<IdTex>> FloorRow::floorTextures;
vector<IdMesh> FloorRow::enemyMeshes;
IdTex FloorRow::planeWood;
IdTex FloorRow::water;
const Assets* FloorRow::assets;