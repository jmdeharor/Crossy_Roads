#include "BasicMeshConfig.h"
using namespace std;
using namespace glm;

MeshConfig BasicMeshConfig::getMeshConfig() const {
	return meshConfig;
}

void BasicMeshConfig::store(ofstream& stream) {
	MeshConfigConstructorType type = MeshConfigConstructorType::Basic;
	stream.write((const char*)&type, sizeof(MeshConfigConstructorType));
	stream.write((const char*)&meshConfig, sizeof(MeshConfig));
	stream.write((const char*)meshConfig.collisionMap, sizeof(bool)*meshConfig.rows*meshConfig.cols);
}

void BasicMeshConfig::load(ifstream& stream) {
	stream.read((char*)&meshConfig, sizeof(MeshConfig));
	bool* collisionMap = new bool[meshConfig.rows*meshConfig.cols];
	stream.read((char*)collisionMap, sizeof(bool)*meshConfig.rows*meshConfig.cols);
	meshConfig.collisionMap = collisionMap;
}

uint BasicMeshConfig::getRows() const {
	return meshConfig.rows;
}

uint BasicMeshConfig::getCols() const {
	return meshConfig.cols;
}

BasicMeshConfig::~BasicMeshConfig() {
	delete meshConfig.collisionMap;
}