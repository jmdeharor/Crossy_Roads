#pragma once
#include "ImportedMesh.h"
#include "CubeMesh.h"
#include "Texture.h"
#include <map>

typedef int IdMesh;
typedef int IdTex;

#define INVALID -1

enum TextureFilter {
	LINEAR,
	NEAREST
};

class Assets {
	ImportedMesh* meshes;
	Texture* textures;
	CubeMesh cubeMesh;
	glm::uint nImportedMeshes;
	glm::uint nTextures;
	std::map<string, IdTex> textureIds;
	std::map<string, IdMesh> meshIds;
public:
	const CubeMesh* getCubeMesh() const;
	IdMesh getMeshId(const string& name) const;
	IdTex getTextureId(const string& name) const;
	const ImportedMesh* getMesh(IdMesh id) const;
	const Texture* getTexture(IdTex id) const;
	glm::uint getNumMeshes() const;
	glm::uint getNumTextures() const;
	void loadAssets(const string& assetsFile);
	Assets();
	~Assets();
};

