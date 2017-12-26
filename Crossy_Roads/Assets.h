#pragma once
#include "ImportedMesh.h"
#include "RandomPickMesh.h"
#include "CubeMesh.h"
#include "Texture.h"
#include <map>
#include "SoundManager.h"

#define nGroups 2

enum AssetType {
	Enemy,
	Platform,
	Decoration,
	Random,
	Unique
};

struct MeshConfig {
	glm::uint rows, cols;
	float height;
	IdMesh mesh;
	bool floorEmpty;
};

class Assets {
	std::vector<IdMesh> groups[nGroups];
	std::vector<MeshConfig> decorationGroup;
	std::map<string, RandomPickMesh> randomGroup;
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
	const Texture* getTexture(const string& name) const;
	const RandomPickMesh* getRandomMesh(const string& name) const;
	const std::vector<IdMesh>* getGroups() const;
	const std::vector<MeshConfig>* getDecoration() const;
	glm::uint getNumMeshes() const;
	glm::uint getNumTextures() const;
	void loadAssets(const string& modelPath, const string& texturePath);
	Assets();
	~Assets();
};

