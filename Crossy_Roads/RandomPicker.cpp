#include "RandomPicker.h"
using namespace glm;

void RandomPicker::setProbabilities(const float * probabilities, uint size) {
	this->probabilities = probabilities;
	this->size = size;
}

uint RandomPicker::getIndex() const {
	float pick = rand() / (float)(RAND_MAX+1);
	float prob = 0;
	for (uint i = 0; i < size; ++i) {
		if (pick >= prob && pick < prob + probabilities[i]) {
			return i;
		}
		prob += probabilities[i];
	}
	return 0;
}

RandomPicker::RandomPicker() : probabilities(NULL)
{
}

RandomPicker::~RandomPicker() {
	if (probabilities != NULL) {
		delete probabilities;
	}
}
