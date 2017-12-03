#pragma once
#include "Mesh.h"
#include "Texture.h"
#include <assimp/scene.h>

class ImportedMesh : public Mesh {
	Texture texture;
	std::vector<glm::vec3> vertices, normals;
	std::vector<glm::vec2> texCoords;
	std::vector<unsigned int> triangles;

	void prepareArrays();
	void computeBoundingBox();
	void initMesh(const aiMesh *paiMesh);
	bool initMaterials(const aiScene *pScene, const string &filename);
public:
	bool loadFromFile(const string &filename);
	void useTexture() const;
	//void render(ShaderProgram& shaderProgram) const override;
	ImportedMesh();
	~ImportedMesh();
};

