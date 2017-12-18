/*#include "Biome.h"
using namespace glm;

const vector<vector<string>> Biome::models = {
	{ "wood_3_1", "wood_3_2", "wood_3_3", "wood_3_4", "wood_3_5" },
	{ "wood_4_0", "wood_4_1", "wood_4_2", "wood_4_3", "wood_4_4" },
	{ "wood_5_0", "wood_5_1", "wood_5_2", "wood_5_3", "wood_5_4" },
	{ "wood_6_0", "wood_6_1", "wood_6_2", "wood_6_3", "wood_6_4" },
	{ "wood_7_0", "wood_7_1", "wood_7_2", "wood_7_3", "wood_7_4" } };

const vector<string> Biome::enemyMeshesStrings = {
	"pirate", "pirate_2"
};

void Biome::initIds(const Assets & assets) {
	Biome::assets = &assets;
	enemyMeshes.resize(enemyMeshesStrings.size());
	for (uint i = 0; i < enemyMeshesStrings.size(); ++i) {
		enemyMeshes[i] = assets.getMeshId(enemyMeshesStrings[i]);
	}

	indices.resize(models.size());
	for (uint i = 0; i < indices.size(); ++i) {
		indices[i] = i;
	}

	floorTextures.resize(models.size());
	for (uint i = 0; i < models.size(); ++i) {
		floorTextures[i].resize(models[i].size());
		for (uint j = 0; j < models[i].size(); ++j) {
			floorTextures[i][j] = assets.getTextureId(models[i][j]);
		}
	}
	planeWood = assets.getTextureId("wood_plane");
	water = assets.getTextureId("water_3");
	crocodile = assets.getMeshId("crocodile-01");
	shark = assets.getMeshId("shark-00");
}

void Biome::setParameters(const FloorParams& floorParams) {
	fp = floorParams;
}

void Biome::initSafeZone(const std::vector<CellProperties>& map) {
}

Biome::Biome(RowVectors& vectors) : v(vectors) {
}

Biome::~Biome() {
}

FloorParams Biome::fp;
IdMesh Biome::crocodile;
IdMesh Biome::shark;
vector<vector<IdTex>> Biome::floorTextures;
vector<IdMesh> Biome::enemyMeshes;
IdTex Biome::planeWood;
IdTex Biome::water;
const Assets* Biome::assets;
std::vector<glm::uint> Biome::indices;*/