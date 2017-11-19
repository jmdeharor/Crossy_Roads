#include "Floor.h"
using namespace glm;

glm::vec2 Floor::getOffsets() {
	float realTileSize = tileSize.x / cols;
	float offsetZ = -tileSize.y*(rows / 2);
	float offsetX = -(realTileSize*(cols / 2) - (1 - cols % 2)*realTileSize / 2);
	return vec2(offsetX, offsetZ);
}

uint Floor::getNumRows() {
	return rows;
}

void Floor::addLevel() {
	firstPos += tileSize.y;
	for (uint i = 0; i < cols; ++i) {
		Object& tile = planeTiles[lastRow*cols + i];
		vec3 tilePos = tile.getPos();
		tile.setPos(vec3(tilePos.x, 0, firstPos));
		tile.updateModel();
	}
	if (lastRow == rows-1) {
		lastRow = 0;
	}
	else {
		++lastRow;
	}
}

void Floor::firstInit() {
	floorPlane.init();
	enemyMesh.loadFromFile("models/pirate.obj");
	rows = 31;
	cols = 5;
	planeTiles.resize(rows*cols);
	enemies.resize(rows);
	floorPlane.setQuadTexture("images/toon road texture_img.png");
	tileSize = vec2(60, 2);
}

vec2 Floor::getTileSize() {
	return tileSize;
}

void Floor::setLight(glm::vec3 lightDir) {
	this->lightDir = lightDir;
}

void Floor::init() {
	GameObject::init();
	float realTileSize = tileSize.x / cols;

	float offsetZ = -tileSize.y*(rows/2);
	float offsetX = -(realTileSize*(cols / 2) - (1-cols%2)*realTileSize/2);

	for (uint i = 0; i < rows; ++i) {
		ShadowedObject& enemy = enemies[i];
		enemy.setMesh(&enemyMesh);
		enemy.name = "enemy " + to_string(i);
		enemy.rotateY(PI);
		enemy.setScale(vec3(0.1f));
		enemy.setPos(vec3(-30 + ((float)rand() / RAND_MAX) * 60, enemy.getHeight() / 2, offsetZ + i*tileSize.y));
		enemy.setPlane(vec4(0, 1, 0, 0), lightDir);
		enemy.updateModel();
	}

	for (uint i = 0; i < rows; ++i) {
		for (uint j = 0; j < cols; ++j) {
			Object& tile = planeTiles[i*cols + j];
			tile.name = "floor tile " + to_string(i);
			tile.setMesh(&floorPlane);
			tile.setScale(vec3(realTileSize/2, 1, tileSize.y / 2));
			tile.setPos(vec3(offsetX + j*realTileSize, 0, offsetZ + i*tileSize.y));
			tile.updateModel();
		}
	}
	lastRow = 0;
	firstPos = planeTiles[planeTiles.size()-1].getPos().z;
}

void Floor::addObjects(Renderer & renderer) {
	renderer.addGroup(&enemies[0], enemies.size(), sizeof(enemies[0]));
	renderer.addSimpleGroup(&planeTiles[0], planeTiles.size(), sizeof(planeTiles[0]));
}

void Floor::update(int deltaTime) {
	for (uint i = 0; i < enemies.size(); ++i) {
		Object& object = enemies[i];
		object.move(0.1f, 0, 0);
		if (object.getPos().x > 30) {
			object.setPos(vec3(-30, object.getPos().y, object.getPos().z));
		}
		object.updateModel();
	}
}

Floor::Floor()
{
}

Floor::~Floor()
{
}