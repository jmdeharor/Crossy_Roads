#pragma once
#include "Object.h"
#include "Texture.h"

class TexturedObject : public Object {
	const Texture* texture;
public:
	void render(ShaderProgram& program) override;
	void setTexture(const Texture* texture);
	TexturedObject();
	~TexturedObject();
};

