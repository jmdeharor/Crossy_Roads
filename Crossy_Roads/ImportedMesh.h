#pragma once
#include "Mesh.h"
#include "Texture.h"
#include <assimp/scene.h>
#include <fstream>

class ImportedMesh : public Mesh {
	Texture texture;
	glm::vec3* vertices;
	glm::vec3* normals;
	glm::vec2* texCoords;
	glm::uint* triangles;
	glm::uint nModelVertices;

	void prepareArrays(std::ifstream& input);
	void prepareArrays(std::ofstream& output);
	void computeBoundingBox();
	void initMesh(const aiMesh *paiMesh);
	bool initMaterials(const aiScene *pScene, const string &filename, string& texturePath);
public:
	bool loadFromFile(const string &filename);
	bool loadFromBinary(const string& filename);
	void useTexture() const;
	ImportedMesh();
	~ImportedMesh();
};

