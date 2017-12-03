#include <iostream>
#include <SOIL.h>
#include "Texture.h"


using namespace std;


Texture::Texture()
{
	texId = 0;
	wrapS = GL_REPEAT;
	wrapT = GL_REPEAT;
	minFilter = GL_LINEAR_MIPMAP_LINEAR;
	magFilter = GL_LINEAR;
}

Texture::~Texture() {
	glDeleteTextures(1, &texId);
}


bool Texture::loadFromFile(const string &filename, PixelFormat format, bool mipmap)
{
	unsigned char *image = NULL;
	
	switch(format)
	{
	case TEXTURE_PIXEL_FORMAT_RGB:
		image = SOIL_load_image(filename.c_str(), &widthTex, &heightTex, 0, SOIL_LOAD_RGB);
		break;
	case TEXTURE_PIXEL_FORMAT_RGBA:
		image = SOIL_load_image(filename.c_str(), &widthTex, &heightTex, 0, SOIL_LOAD_RGBA);
		break;
	}
	if(image == NULL)
		return false;
	if (texId == 0)
		glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	switch(format)
	{
	case TEXTURE_PIXEL_FORMAT_RGB:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthTex, heightTex, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		break;
	case TEXTURE_PIXEL_FORMAT_RGBA:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthTex, heightTex, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		break;
	}
	if (mipmap)
		glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	return true;
}

void Texture::loadFromGlyphBuffer(unsigned char *buffer, int width, int height)
{
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, buffer);
	glGenerateMipmap(GL_TEXTURE_2D);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

void Texture::createEmptyTexture(int width, int height)
{
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

void Texture::loadSubtextureFromGlyphBuffer(unsigned char *buffer, int x, int y, int width, int height)
{
	glBindTexture(GL_TEXTURE_2D, texId);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, GL_RED, GL_UNSIGNED_BYTE, buffer);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

void Texture::generateMipmap()
{
	glBindTexture(GL_TEXTURE_2D, texId);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenerateMipmap(GL_TEXTURE_2D);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

void Texture::setWrapS(GLint value)
{
	wrapS = value;
}

void Texture::setWrapT(GLint value)
{
	wrapT = value;
}

void Texture::setMinFilter(GLint value)
{
	minFilter = value;
}

void Texture::setMagFilter(GLint value)
{
	magFilter = value;
}

void Texture::applyParams() const {
	use();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
}


void Texture::use() const {
	glBindTexture(GL_TEXTURE_2D, texId);
}


