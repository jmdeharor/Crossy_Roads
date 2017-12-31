#pragma once
#include <glm\glm.hpp>
typedef int IdMesh;
typedef int IdTex;

#define INVALID -1

class RandomPicker {
	glm::uint size;
	const float* probabilities;
public:
	void setProbabilities(const float* probabilities, glm::uint size);
	glm::uint getIndex() const;
	RandomPicker();
	~RandomPicker();
};

