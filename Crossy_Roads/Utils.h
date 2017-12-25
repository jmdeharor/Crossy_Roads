#pragma once
#include <iostream>
#define PI 3.141592654f

const float maxSpeed = 0.3f;
const float minSpeed = 0.05f;

inline float generateSpeed() {
	return (((float)rand() / RAND_MAX)*(maxSpeed - minSpeed) + minSpeed)*(-1 + (rand() % 2) * 2);
}

inline float generateSpeed(float minSpeed, float maxSpeed, bool sign) {
	return (((float)rand() / RAND_MAX)*(maxSpeed - minSpeed) + minSpeed)*(-1 + sign * 2);
}

inline int between(int min, int max) {
	float num = ((float)rand() / RAND_MAX)*(max - min) + min;
	return (int)round(num);
}

inline float between(float min, float max) {
	float num = ((float)rand() / RAND_MAX)*(max - min) + min;
	return num;
}