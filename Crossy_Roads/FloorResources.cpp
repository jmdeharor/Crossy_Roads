#include "FloorResources.h"
using namespace glm;

void FloorResources::init(const Assets & assets, const Player* player) {
	this->player = player;
	this->assets = &assets;
	meshGroups = assets.getGroups();

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
	
	waterAnimation = assets.getAnimatedTexture("water_floor");
	sharkAnimation = assets.getAnimatedMesh("shark");
	islandRoad = assets.getTextureId("sand_texture_2");
	islandSafe = assets.getTextureId("grass_texture");
}

FloorResources::FloorResources() : 
	models({
	{ "wood_3_1", "wood_3_2", "wood_3_3", "wood_3_4", "wood_3_5" },
	{ "wood_4_0", "wood_4_1", "wood_4_2", "wood_4_3", "wood_4_4" },
	{ "wood_5_0", "wood_5_1", "wood_5_2", "wood_5_3", "wood_5_4" },
	{ "wood_6_0", "wood_6_1", "wood_6_2", "wood_6_3", "wood_6_4" },
	{ "wood_7_0", "wood_7_1", "wood_7_2", "wood_7_3", "wood_7_4" } })
{
}

FloorResources::~FloorResources()
{
}