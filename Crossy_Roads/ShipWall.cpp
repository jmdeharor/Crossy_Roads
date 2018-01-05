#include "ShipWall.h"
#include "FloorRow.h"
using namespace glm;

void ShipWall::clear() {
	wall.clear();
}

void ShipWall::init(const Assets& assets, const FloorParams & fp, vec3 floorPos, float wallHeight, uint rows) {
	float rowHeight = wallHeight/rows;
	vec3 offset = vec3(floorPos.x, floorPos.y - rowHeight / 2, floorPos.z - fp.realTileSize / 2);

	const Mesh* cubeMesh = assets.getCubeMesh();
	vec3 bbcenter = cubeMesh->getbbCenter();
	vec3 bbSize = cubeMesh->getbbSize();
	IdTex wallTex = assets.getTextureId("wood_plane");
	const static float wallWidth = 0.2f;
	vec3 scale = vec3(fp.tileSize.x, wallHeight, wallWidth) / bbSize;
	wall.resize(rows);
	for (uint i = 0; i < rows; ++i) {
		TexturedObject& object = wall[i];
		object.texture = wallTex;
		object.setMesh(cubeMesh);
		object.setScale(scale);
		object.setCenter(vec3(bbcenter.x, bbcenter.y, bbcenter.z + bbSize.z/2));
		object.setPos(vec3(offset.x, offset.y - i*rowHeight, offset.z));
	}
}

void ShipWall::groupDrawableObjects(const FrustumG & frustum, RenderVectors & renderVectors) {
	for (uint i = 0; i < wall.size(); ++i) {
		if (wall[i].isInsideViewFrustrum(frustum)) {
			renderVectors.texturedObjects[wall[i].texture].push_back(&wall[i]);
		}
	}
}

ShipWall::ShipWall()
{
}


ShipWall::~ShipWall()
{
}
