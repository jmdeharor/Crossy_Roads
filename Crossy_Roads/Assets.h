#pragma once
#include "ImportedMesh.h"
#include "RandomPicker.h"
#include "CubeMesh.h"
#include "Texture.h"
#include <map>
#include "SoundManager.h"
#include "BasicMeshConfig.h"
#include "RandomMeshConfig.h"

#define nMeshGroups 4
#define nMeshConfigGroups 3
#define nBiomes 3

enum class MeshGroup {
	Enemy,
	Platform,
	Player,
	Unique
};

enum class MeshConfigGroup {
	Decoration,
	Border,
	Unique
};

enum BiomeType {
	Ship,
	Sea,
	Island
};

enum class MonoBehaviourType {
	Stalker,
	Coin,
	None
};

inline glm::uint sub2ind(BiomeType biome, MeshGroup group) {
	return nMeshGroups*biome + (int)group;
}

inline glm::uint sub2ind(BiomeType biome, MeshConfigGroup group) {
	return nMeshConfigGroups*biome + (int)group;
}

class Assets {
	//Groups
	std::vector<IdMesh> meshGroups[nBiomes][nMeshGroups];
	std::vector<MeshConfigConstructor*> meshConfigGroups[nBiomes][nMeshConfigGroups];
	std::map<string, std::pair<IdTex, glm::uint>> animatedTextureGroup;
	std::map<string, std::pair<IdTex, glm::uint>> animatedMeshGroup;

	//Memory
	float* scales;
	RandomMeshConfig* randomMeshConfigs;
	BasicMeshConfig* basicMeshConfigs;
	ImportedMesh* meshes;
	Texture* textures;
	MonoBehaviourType* behaviours;
	CubeMesh cubeMesh;

	glm::uint nImportedMeshes;
	glm::uint nTextures;
	glm::uint nRandomMeshConfigs;
	glm::uint nbasicMeshConfigs;
	std::map<string, IdTex> textureIds;
	std::map<string, IdMesh> meshIds;
	std::map<string, MeshConfigConstructor*> meshConfigIds;

public:
	IdMesh getMeshId(const string& name) const;
	IdTex getTextureId(const string& name) const;
	const Texture* getTexture(const string& name) const;
	const Mesh* getMesh(const string& name) const;
	const MeshConfigConstructor* getMeshConfigId(const string& name) const;
	
	float getScale(IdMesh id) const;
	const CubeMesh* getCubeMesh() const;
	const ImportedMesh* getMesh(IdMesh id) const;
	const Texture* getTexture(IdTex id) const;

	std::pair<IdTex, glm::uint> getAnimatedTexture(const string& name) const;
	std::pair<IdMesh, glm::uint> getAnimatedMesh(const string& name) const;

	const std::vector<IdMesh>* getGroups() const;
	const std::vector<MeshConfigConstructor*>* getMeshConfigGroups() const;

	MonoBehaviourType getBehavior(IdMesh mesh) const;

	glm::uint getNumMeshes() const;
	glm::uint getNumTextures() const;
	void loadAssets(const string& modelPath, const string& texturePath);
	void loadAssets(const string& binaryPath);
	Assets();
	~Assets();
};

