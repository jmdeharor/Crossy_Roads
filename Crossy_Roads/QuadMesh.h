#pragma once
#include "Mesh.h"

class QuadMesh : public Mesh {
public:
	void init();
	void render(ShaderProgram& program) const;
	void setQuadTexture(const string& fileName);
	QuadMesh();
	~QuadMesh();
};

