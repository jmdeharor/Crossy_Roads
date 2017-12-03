#include "TexturedObject.h"

void TexturedObject::render(ShaderProgram & program) {
	//texture->use();
	Object::render(program);
}

TexturedObject::TexturedObject()
{
}


TexturedObject::~TexturedObject()
{
}
