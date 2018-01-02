#pragma once
#include <glm\glm.hpp>

class RandomPicker {
	glm::uint size;
	const float* probabilities;
public:
	void setProbabilities(const float* probabilities, glm::uint size);
	glm::uint getIndex() const;
	glm::uint getSize() const;
	const float* getProbabilities() const;
	RandomPicker();
	~RandomPicker();
};

