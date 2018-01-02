#include "RandomMeshConfig.h"
using namespace std;
using namespace glm;

void RandomMeshConfig::setProbabilities(const float* probabilities, glm::uint size) {
	randomPicker.setProbabilities(probabilities, size);
}
void RandomMeshConfig::store(ofstream& stream) {
	MeshConfigConstructorType type = MeshConfigConstructorType::Random;
	stream.write((const char*)&type, sizeof(MeshConfigConstructorType));

	stream.write((const char*)&firstMesh, sizeof(firstMesh));
	stream.write((const char*)&rows, sizeof(rows));
	stream.write((const char*)&cols, sizeof(cols));
	stream.write((const char*)&canJump, sizeof(canJump));
	uint length = randomPicker.getSize();
	stream.write((const char*)&length, sizeof(uint));
	stream.write((const char*)randomPicker.getProbabilities(), sizeof(float)*length);
	stream.write((const char*)heights, sizeof(float)*length);
	stream.write((const char*)empty, sizeof(bool)*length);
	stream.write((const char*)collisionMap, sizeof(bool)*rows*cols);
}
void RandomMeshConfig::load(ifstream& stream) {
	stream.read((char*)&firstMesh, sizeof(firstMesh));
	stream.read((char*)&rows, sizeof(rows));
	stream.read((char*)&cols, sizeof(cols));
	stream.read((char*)&canJump, sizeof(canJump));
	uint length;
	stream.read((char*)&length, sizeof(uint));
	float* probabilities = new float[length];
	stream.read((char*)probabilities, sizeof(float)*length);
	heights = new float[length];
	stream.read((char*)heights, sizeof(float)*length);
	empty = new bool[length];
	stream.read((char*)empty, sizeof(bool)*length);
	collisionMap = new bool[rows*cols];
	stream.read((char*)collisionMap, sizeof(bool)*rows*cols);
	randomPicker.setProbabilities(probabilities, length);
}
MeshConfig RandomMeshConfig::getMeshConfig() const {
	glm::uint index = randomPicker.getIndex();
	MeshConfig meshConfig;
	meshConfig.mesh = firstMesh + index;
	meshConfig.height = heights[index];
	meshConfig.floorEmpty = empty[index];
	meshConfig.rows = rows;
	meshConfig.cols = cols;
	meshConfig.collisionMap = collisionMap;
	meshConfig.canJump = canJump;
	return meshConfig;
}
uint RandomMeshConfig::getRows() const {
	return rows;
}
uint RandomMeshConfig::getCols() const {
	return cols;
}

RandomMeshConfig::~RandomMeshConfig() {
	delete heights;
	delete empty;
	delete collisionMap;
}
