#pragma once
#include "Mesh.h"

class Sphere : public Mesh {
	int size;
public:
	void init(int numCircles, int pointsPerCircle, float diameter, ShaderProgram& program);
	void render();
	Sphere();
	~Sphere();
};

