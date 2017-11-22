#include "TexturedObject.h"

void TexturedObject::render(ShaderProgram & program) {
	texture->use();
	Object::render(program);
}

void TexturedObject::setTexture(const Texture * texture) {
	this->texture = texture;
}

TexturedObject::TexturedObject()
{
}


TexturedObject::~TexturedObject()
{
}
