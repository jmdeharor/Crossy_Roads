#include "FloorRow.h"
#include "Pi.h"
using namespace glm;

void FloorRow::initMeshes() {
	pirateMesh.loadFromFile("models/pirate.obj");
	mastMesh.loadFromFile("models/palo.obj");
	floorMesh.loadFromFile("models/floor_1.obj");
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
		velocities[i] = generateSpeed();
	}
	pos = position;
}

void FloorRow::setPos(glm::vec2 position) {
	pos = position;
}

void FloorRow::init() {
	enemies.resize(1);
	velocities.resize(enemies.size());
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
		velocities[i] = generateSpeed();
	}
	static vec3 boundingBox = floorMesh.getbbSize();
	static vec3 floorTileSize = vec3(realTileSize, 0.1f, tileSize.y) / boundingBox;
	for (uint i = 0; i < floorTiles.size(); ++i) {
		Object& tile = floorTiles[i];
		tile.name = "floor tile " + to_string(i);
		tile.setMesh(&floorMesh);
		tile.setScale(floorTileSize);
		tile.rotateY(PI / 2);
		tile.setPos(vec3(offsetX + i*realTileSize, -boundingBox.y*0.1f / 2, pos.y));
	}
}

void FloorRow::update(int deltaTime) {
	for (uint i = 0; i < enemies.size(); ++i) {
		Object& object = enemies[i];
		object.move(velocities[i], 0, 0);
		if (object.getPos().x > tileSize.x / 2) {
			velocities[i] = generateSpeed();
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
}

void FloorRow::renderShadows(ShaderProgram & program) {
	for (ShadowedObject& object : enemies) {
		object.renderShadow(program);
	}
}

FloorRow::FloorRow()
{
}


FloorRow::~FloorRow()
{
}

glm::vec2 FloorRow::tileSize;
uint FloorRow::cols;
glm::vec3 FloorRow::lightDir;
Mesh FloorRow::pirateMesh;
Mesh FloorRow::mastMesh;
Mesh FloorRow::floorMesh;