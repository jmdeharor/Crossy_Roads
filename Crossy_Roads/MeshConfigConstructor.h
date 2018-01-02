#pragma once
#include <glm\glm.hpp>
#include <fstream>

typedef int IdMesh;
typedef int IdTex;

#define INVALID -1

struct MeshConfig {
	glm::uint rows, cols;
	IdMesh mesh;
	float height;
	bool floorEmpty;
	bool* collisionMap;
	bool canJump;
};

enum class MeshConfigConstructorType {
	Basic,
	Random
};

class MeshConfigConstructor {
public:
	virtual MeshConfig getMeshConfig() const = 0;
	virtual glm::uint getRows() const = 0;
	virtual glm::uint getCols() const = 0;
	virtual void store(std::ofstream& stream) = 0;
	virtual void load(std::ifstream& stream) = 0;
	virtual ~MeshConfigConstructor();
};
