#pragma once
#include "Object.h"
#include "Texture.h"

class TexturedObject : public Object {
public:
	IdTex texture;

	void render(ShaderProgram& program) override;
	TexturedObject();
	~TexturedObject();
};

