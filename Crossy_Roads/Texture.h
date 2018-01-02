#ifndef _TEXTURE_INCLUDE
#define _TEXTURE_INCLUDE


#include <string>
#include <GL/glew.h>


using namespace std;


enum PixelFormat {TEXTURE_PIXEL_FORMAT_RGB, TEXTURE_PIXEL_FORMAT_RGBA};


// The texture class loads images an passes them to OpenGL
// storing the returned id so that it may be applied to any drawn primitives


class Texture
{

public:
	Texture();
	~Texture();

	bool loadFromFile(const string &filename, PixelFormat format, bool mipmap);
	void loadFromGlyphBuffer(unsigned char *buffer, int width, int height);

	void createEmptyTexture(int width, int height);
	void loadSubtextureFromGlyphBuffer(unsigned char *buffer, int x, int y, int width, int height);
	void generateMipmap();

	void applyParams() const;
	
	void use() const;
	
	int width() const { return widthTex; }
	int height() const { return heightTex; }

	bool hasMipmap() const;

	GLint wrapS, wrapT, minFilter, magFilter;
private:
	string extension;
	int widthTex, heightTex;
	bool mipmap;
	GLuint texId;

};


#endif // _TEXTURE_INCLUDE

