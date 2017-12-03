#pragma once
#include "ImportedMesh.h"
#include "CubeMesh.h"
#include "Texture.h"

typedef int IdMesh;
typedef int IdTex;

#define INVALID -1

enum TextureFilter {
	LINEAR,
	NEAREST
};

class Assets {
	ImportedMesh* meshes;
	CubeMesh cubeMesh;
	uint nImportedMeshes;
	Texture* textures;
	std::map<string, IdTex> textureIds;
	std::map<string, IdMesh> meshIds;
public:
	const CubeMesh* getCubeMesh() const;
	IdMesh getMeshId(const string& name) const;
	IdTex getTextureId(const string& name) const;
	const ImportedMesh* getMesh(IdMesh id) const;
	const Texture* getTexture(IdTex id) const;
	void loadAssets(const string * meshNames, const string * textureNames,
		const TextureFilter* textureFilters, uint nMeshes, uint nTextures);
	Assets();
	~Assets();
};

