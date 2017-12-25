#pragma once
#include "Mesh.h"

class CubeMesh : public Mesh {
protected:
	GLuint VBOind;
public:
	void render() const override;
	void setProgramParams(ShaderProgram& program) const override;
	void init();
	CubeMesh();
	~CubeMesh();
};

