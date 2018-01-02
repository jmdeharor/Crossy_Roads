#pragma once
#include "ImportedMesh.h"
#include "RandomPicker.h"
#include "CubeMesh.h"
#include "Texture.h"
#include <map>
#include "SoundManager.h"
#include "BasicMeshConfig.h"
#include "RandomMeshConfig.h"

#define nGroups 3
#define nBiomes 3

enum AssetType {
	Enemy,
	Platform,
	Decoration,
	Random,
	Unique
};

enum BiomeType {
	Ship,
	Sea,
	Island
};

enum class MeshBehavior {
	Stalker,
	None
};

inline glm::uint sub2ind(BiomeType biome, AssetType asset) {
	return nGroups*biome + asset;
}

class Assets {
	std::vector<IdMesh> groups[nBiomes][nGroups];
	std::vector<MeshConfigConstructor*> decorationGroup[nBiomes];
	std::map<string, RandomMeshConfig> randomGroup;
	std::map<string, std::pair<IdTex, glm::uint>> animatedTextureGroup;
	std::map<string, std::pair<IdTex, glm::uint>> animatedMeshGroup;
	ImportedMesh* meshes;
	Texture* textures;
	MeshBehavior* behaviours;
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
	const RandomMeshConfig* getRandomMesh(const string& name) const;
	std::pair<IdTex, glm::uint> getAnimatedTexture(const string& name) const;
	std::pair<IdMesh, glm::uint> getAnimatedMesh(const string& name) const;
	const std::vector<IdMesh>* getGroups() const;
	const std::vector<MeshConfigConstructor*>* getDecoration() const;
	MeshBehavior getBehavior(IdMesh mesh) const;
	glm::uint getNumMeshes() const;
	glm::uint getNumTextures() const;
	void loadAssets(const string& modelPath, const string& texturePath);
	void loadAssets(const string& binaryPath);
	Assets();
	~Assets();
};

