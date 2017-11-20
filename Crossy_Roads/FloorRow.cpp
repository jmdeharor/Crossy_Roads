#include "FloorRow.h"
#include "Pi.h"
using namespace glm;

const string models[] = { "models/floor_2.obj","models/floor_3.obj","models/floor_4.obj" };
const uint nModels = sizeof(models) / sizeof(string);

void FloorRow::initMeshes() {
	pirateMesh.loadFromFile("models/pirate.obj");
	mastMesh.loadFromFile("models/palo.obj");
	for (uint i = 0; i < nModels; ++i) {
		floorMesh[i].loadFromFile(models[i]);
	}
}

void FloorRow::setParameters(vec2 tileSize, uint cols, vec3 lightDir) {
	FloorRow::tileSize = tileSize;
	FloorRow::cols = cols;
	FloorRow::lightDir = lightDir;
}

glm::vec2 FloorRow::getPos() const {
	return pos;
}

float generateSpeed() {
	const static float maxSpeed = 0.3f;
	const static float minSpeed = 0.05f;
	return ((float)rand() / RAND_MAX)*(maxSpeed - minSpeed) + minSpeed;
}

void FloorRow::moveToPosition(glm::vec2 position) {
	static float realTileSize = tileSize.x / cols;
	float offsetX = pos.x - (realTileSize*(cols / 2) - (1 - cols % 2)*realTileSize / 2);
	for (uint i = 0; i < floorTiles.size(); ++i) {
		Object& tile = floorTiles[i];
		vec3 tilePos = tile.getPos();
		tile.setPos(vec3(tilePos.x, tilePos.y, position.y));
	}
	for (uint i = 0; i < enemies.size(); ++i) {
		enemies[i].setPos(vec3(-30 + ((float)rand() / RAND_MAX) * 60, enemies[i].getHeight() / 2, position.y));
		speeds[i] = generateSpeed();
	}
	pos = position;
}

void FloorRow::setPos(glm::vec2 position) {
	pos = position;
}

bool applyConstraints(uint prevMeshIndex, uint meshIndex, uint numAdjacents, vector<uint>& adjacentRow, uint position) {
	if (prevMeshIndex == meshIndex)
		return false;
	if (position + numAdjacents < adjacentRow.size() && adjacentRow[position + numAdjacents - 1] != adjacentRow[position + numAdjacents])
		return false;
	if (meshIndex == adjacentRow[position])
		return false;
	return true;
}

uint minim(uint a, uint b) {
	return a < b ? a : b;
}

void FloorRow::init(vector<uint>& adjacentRow) {
	enemies.resize(1);
	speeds.resize(enemies.size());
	floorTiles.resize(cols);

	static float realTileSize = tileSize.x / cols;
	float offsetX = pos.x -(realTileSize*(cols / 2) - (1 - cols % 2)*realTileSize / 2);

	for (uint i = 0; i < enemies.size(); ++i) {
		ShadowedObject& enemy = enemies[i];
		enemy.setMesh(&pirateMesh);
		enemy.name = "enemy " + to_string(i);
		enemy.rotateY(PI);
		enemy.setScale(vec3(0.1f));
		enemy.setPos(vec3(-tileSize.x / 2 + ((float)rand() / RAND_MAX) * tileSize.x, enemy.getHeight() / 2, pos.y));
		enemy.setPlane(vec4(0, 1, 0, 0), lightDir);
		speeds[i] = generateSpeed();
	}
	static vec3 boundingBox = floorMesh[0].getbbSize();
	static vec3 floorTileSize = vec3(realTileSize, 0.1f, tileSize.y) / boundingBox;
	uint meshIndex = nModels;
	Mesh* mesh = NULL;
	uint numAdjacentTiles = rand()%6 + 5;
	uint counter = numAdjacentTiles;
	for (uint i = 0; i < floorTiles.size(); ++i) {
		Object& tile = floorTiles[i];
		tile.name = "floor tile " + to_string(i);
		if (counter == numAdjacentTiles) {
			uint prevMeshIndex = meshIndex;
			meshIndex = rand() % nModels;
			numAdjacentTiles = minim(rand() % 5 + 5, floorTiles.size()-i);
			while (!applyConstraints(prevMeshIndex, meshIndex, numAdjacentTiles, adjacentRow, i)) {
				meshIndex = rand() % nModels;
				numAdjacentTiles = rand() % 5 + 5;
			}
			mesh = &floorMesh[meshIndex];
			counter = 0;
		}
		adjacentRow[i] = meshIndex;
		tile.setMesh(mesh);
		tile.setScale(floorTileSize);
		tile.rotateY(PI / 2);
		tile.setPos(vec3(offsetX + i*realTileSize, -boundingBox.y*0.1f / 2, pos.y));
		++counter;
	}
	static float mastHeight = mastMesh.getHeight()*0.5f;
	if (false && rand() % 8 == 0) {
		hasMast = true;
		mast.setMesh(&mastMesh);
		mast.setScale(vec3(0.1f,0.5f,0.1f));
		mast.setPos(vec3(20 -tileSize.x / 2 + ((float)rand() / RAND_MAX) * (tileSize.x-40), mastHeight/2, pos.y));
		mast.setPlane(vec4(0, 1, 0, 0), lightDir);
	}
	else {
		hasMast = false;
	}
}

void FloorRow::update(int deltaTime) {
	for (uint i = 0; i < enemies.size(); ++i) {
		Object& object = enemies[i];
		object.move(speeds[i], 0, 0);
		if (object.getPos().x > tileSize.x / 2) {
			speeds[i] = generateSpeed();
			object.setPos(vec3(-tileSize.x / 2, object.getPos().y, object.getPos().z));
		}
	}
}

void FloorRow::renderSimpleObjects(ShaderProgram & program) {
	for (Object& object : floorTiles) {
		object.render(program);
	}
}

void FloorRow::renderLightObjects(ShaderProgram & program) {
	for (Object& object : enemies) {
		object.render(program);
	}
	if (hasMast)
		mast.render(program);
}

void FloorRow::renderShadows(ShaderProgram & program) {
	for (ShadowedObject& object : enemies) {
		object.renderShadow(program);
	}
	if (hasMast)
		mast.renderShadow(program);
}

FloorRow::FloorRow()
{
}


FloorRow::~FloorRow()
{
}

vec2 FloorRow::tileSize;
uint FloorRow::cols;
vec3 FloorRow::lightDir;
Mesh FloorRow::pirateMesh;
Mesh FloorRow::mastMesh;
vector<Mesh> FloorRow::floorMesh(nModels);