#pragma once
#include "Object.h"
#include "Texture.h"

class TexturedObject : public Object {
public:
	IdTex texture;

	TexturedObject();
	~TexturedObject();
};

