#pragma once
#include "Mesh.h"
#include "Texture.h"
#include <assimp/scene.h>

class ImportedMesh : public Mesh {
	Texture texture;
	glm::vec3* vertices;
	glm::vec3* normals;
	glm::vec2* texCoords;
	glm::uint* triangles;
	glm::uint nModelVertices;

	void prepareArrays();
	void computeBoundingBox();
	void initMesh(const aiMesh *paiMesh);
	bool initMaterials(const aiScene *pScene, const string &filename);
public:
	bool loadFromFile(const string &filename);
	void useTexture() const;
	ImportedMesh();
	~ImportedMesh();
};

