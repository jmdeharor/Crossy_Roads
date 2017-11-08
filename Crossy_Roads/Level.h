#ifndef _LEVEL_INCLUDE
#define _LEVEL_INCLUDE


#include <string>
#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"


using namespace std;


// Class Level renders a very simple room with textures


class Level
{

public:
	// Levels can only be created inside an OpenGL context
	static Level *createLevel(const glm::vec3 &levelSize, ShaderProgram &program, const string &floorFile, const string &wallFile);

	Level(const glm::vec3 &levelSize, ShaderProgram &program, const string &floorFile, const string &wallFile);
	~Level();

	void render() const;
	void free();

private:
	void prepareArrays(ShaderProgram &program);

private:
	GLuint vao[2];
	GLuint vbo[2];
	GLint posLocation[2], texCoordLocation[2];
	glm::vec3 size;
	Texture floor, wall;

};


#endif // _LEVEL_INCLUDE


