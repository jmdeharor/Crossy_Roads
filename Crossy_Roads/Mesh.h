#pragma once
#include <vector>
#include <glm\glm.hpp>
#include <GL\glew.h>
#include "ShaderProgram.h"
#include <assimp/scene.h>
#include "Texture.h"

class Mesh {
protected:
	unsigned int nTriangles;
	Texture texture;
	std::vector<glm::vec3> vertices, normals;
	std::vector<glm::vec2> texCoords;
	std::vector<unsigned int> triangles;
	float height;
	glm::vec3 center, bbox[2];
	GLuint VAO;
	GLuint VBOvert, VBOnorm, VBOtex;

	void prepareArrays();
	void computeBoundingBox();
	void initMesh(const aiMesh *paiMesh);
	bool initMaterials(const aiScene *pScene, const string &filename);
public:
	glm::vec3 getbbCenter() const;
	glm::vec3 getbbSize() const;
	float getHeight() const;
	bool loadFromFile(const string &filename);
	virtual void render(ShaderProgram& program) const;
	void clear();
	Mesh();
	virtual ~Mesh();
};

