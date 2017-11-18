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

void Floor::setShaderProgram(ShaderProgram * shaderProgram) {
	program = shaderProgram;
}

void Floor::firstInit() {
	floorPlane.init();
	rows = 31;
	cols = 5;
	planeTiles.resize(rows*cols);
	floorTex.loadFromFile("images/toon road texture_img.png", TEXTURE_PIXEL_FORMAT_RGB);
	tileSize = vec2(60, 2);
}

vec2 Floor::getTileSize() {
	return tileSize;
}

void Floor::init() {
	GameObject::init();
	float realTileSize = tileSize.x / cols;

	float offsetZ = -tileSize.y*(rows/2);
	float offsetX = -(realTileSize*(cols / 2) - (1-cols%2)*realTileSize/2);

	for (uint i = 0; i < rows; ++i) {
		for (uint j = 0; j < cols; ++j) {
			Object& tile = planeTiles[i*cols + j];
			tile.setMesh(&floorPlane);
			tile.setShader(program);
			tile.setScale(vec3(realTileSize/2, 1, tileSize.y / 2));
			tile.setPos(vec3(offsetX + j*realTileSize, 0, offsetZ + i*tileSize.y));
			tile.updateModel();
		}
	}
	lastRow = 0;
	firstPos = planeTiles[planeTiles.size()-1].getPos().z;
}

void Floor::render() {
	floorTex.use();
	for (Object& tile : planeTiles) {
		tile.render();
	}
}

void Floor::update(int deltaTime)
{
}

Floor::Floor()
{
}

Floor::~Floor()
{
}