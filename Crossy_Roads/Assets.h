#pragma once
#include "ImportedMesh.h"
#include "RandomPicker.h"
#include "CubeMesh.h"
#include "Texture.h"
#include <map>
#include "SoundManager.h"

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

struct MeshConfig {
	glm::uint rows, cols;
	IdMesh mesh;
	float height;
	bool floorEmpty;
};

class MeshConfigConstructor {
public:
	virtual MeshConfig getMeshConfig() const = 0;
	virtual glm::uint getRows() const = 0;
	virtual glm::uint getCols() const = 0;
	virtual ~MeshConfigConstructor() {};
};

class BasicMeshConfig : public MeshConfigConstructor {
public:
	MeshConfig meshConfig;
	MeshConfig getMeshConfig() const override {
		return meshConfig;
	}
	glm::uint getRows() const override {
		return meshConfig.rows;
	}
	glm::uint getCols() const override {
		return meshConfig.cols;
	}
};

class RandomMeshConfig : public MeshConfigConstructor {
	RandomPicker randomPicker;
public:
	IdMesh firstMesh;
	float* heights;
	bool* empty;
	glm::uint rows, cols;
	void setProbabilities(const float* probabilities, glm::uint size) {
		randomPicker.setProbabilities(probabilities, size);
	}
	MeshConfig getMeshConfig() const override {
		glm::uint index = randomPicker.getIndex();
		MeshConfig meshConfig;
		meshConfig.mesh = firstMesh + index;
		meshConfig.height = heights[index];
		meshConfig.floorEmpty = empty[index];
		meshConfig.rows = rows;
		meshConfig.cols = cols;
		return meshConfig;
	}
	glm::uint getRows() const override {
		return rows;
	}
	glm::uint getCols() const override {
		return cols;
	}
	~RandomMeshConfig() {
		delete heights;
		delete empty;
	}
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
	MeshBehavior* behaviors;
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
	Assets();
	~Assets();
};

