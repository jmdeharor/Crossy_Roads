#ifndef _BILLBOARD_INCLUDE
#define _BILLBOARD_INCLUDE


#include <string>
#include <glm/glm.hpp>
#include "Texture2.h"
#include "ShaderProgram.h"


using namespace std;


enum BillboardType {BILLBOARD_Y_AXIS, BILLBOARD_CENTER};


// Class Billboard renders a quad oriented to the camera by rotating it around the Y axis


class Billboard
{

public:
	// Billboards can only be created inside an OpenGL context
	static Billboard *createBillboard(const glm::vec2 &quadSize, ShaderProgram &program, const string &textureFile, BillboardType billboardType = BILLBOARD_Y_AXIS);

	Billboard(const glm::vec2 &quadSize, ShaderProgram &program, const string &textureFile, BillboardType billboardType = BILLBOARD_Y_AXIS);
	~Billboard();

	void render(const glm::vec3 &position, const glm::vec3 &eye);
	void free();

	void setType(BillboardType billboardType);

private:
	void prepareArrays(ShaderProgram &program);
	void updateArrays(const glm::vec3 &position, const glm::vec3 &eye);

	void prepareBillboardYAxis(const glm::vec3 &position, const glm::vec3 &eye, vector<float> &vertices);
	void prepareBillboardCenter(const glm::vec3 &position, const glm::vec3 &eye, vector<float> &vertices);

private:
	BillboardType type;
	GLuint vao;
	GLuint vbo;
	GLint posLocation, normalLocation, texCoordLocation;
	glm::vec2 size;
	Texture2 texture;

};


#endif // _BILLBOARD_INCLUDE


