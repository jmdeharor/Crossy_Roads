#include "ShipWall.h"
#include "FloorRow.h"
#include "Utils.h"
using namespace glm;

void ShipWall::clear() {
	wall.clear();
	cannonRow.clear();
	cannons.clear();
}

void ShipWall::init(const Assets& assets, const FloorParams & fp, vec3 floorPos, float wallHeight, uint rows, uint cannonSeparation) {
	float rowHeight = wallHeight/rows;
	vec3 offset = vec3(floorPos.x, floorPos.y - rowHeight / 2, floorPos.z - fp.realTileSize / 2);

	const Mesh* cubeMesh = assets.getCubeMesh();
	vec3 bbcenter = cubeMesh->getbbCenter();
	vec3 bbSize = cubeMesh->getbbSize();
	IdTex wallTex = assets.getTextureId("wood_plane");
	const static float wallWidth = 0.2f;
	vec3 scale = vec3(fp.tileSize.x, rowHeight, wallWidth) / bbSize;
	vec3 superScale = vec3(fp.tileSize.x, wallHeight, wallWidth) / bbSize;
	half = rows / 2;
	wall.resize(rows);
	for (uint i = 0; i < wall.size(); ++i) {
		TexturedObject& object = wall[i];
		object.texture = wallTex;
		object.setMesh(cubeMesh);
		if (i != half) {
			object.setScale(scale);
			object.setPos(vec3(offset.x, offset.y - i*rowHeight, offset.z));
		}
		else {
			object.setScale(superScale);
			object.setPos(vec3(offset.x, offset.y - i*rowHeight - 0.5f, offset.z + 1));
		}
		object.setCenter(vec3(bbcenter.x, bbcenter.y, bbcenter.z + bbSize.z/2));
	}
	const IdMesh cannonId = assets.getMeshId("cannon");
	const Mesh* cannonMesh = assets.getMesh(cannonId);
	float cannonScale = 2*fp.realTileSize / cannonMesh->getbbSize().x;
	uint aux = 0;
	cannons.resize(ceil(fp.cols / (float)cannonSeparation));
	cannonRow.resize(fp.cols - cannons.size());
	scale.x = fp.realTileSize / bbSize.x;
	for (uint i = 0; i < fp.cols; ++i) {
		if (i%cannonSeparation == 0) {
			Object& cannon = cannons[i/cannonSeparation];
			cannon.setMesh(cannonId, cannonMesh);
			cannon.setScale(vec3(cannonScale));
			cannon.setRotationY(PI / 2);
			cannon.setPos(vec3(offset.x + fp.offsetX + i*fp.realTileSize, offset.y - half*rowHeight, offset.z + 0.5f));
		}
		else {
			TexturedObject& object = cannonRow[aux];
			object.texture = wallTex;
			object.setMesh(cubeMesh);
			object.setScale(scale);
			object.setCenter(vec3(bbcenter.x, bbcenter.y, bbcenter.z + bbSize.z / 2));
			object.setPos(vec3(offset.x + fp.offsetX + i*fp.realTileSize, offset.y - half*rowHeight, offset.z));
			++aux;
		}
	}
}

void ShipWall::groupDrawableObjects(const FrustumG & frustum, RenderVectors & renderVectors) {
	for (uint i = 0; i < wall.size(); ++i) {
		//if (i == half)
		//	continue;
		if (wall[i].isInsideViewFrustrum(frustum)) {
			renderVectors.texturedObjects[wall[i].texture].push_back(&wall[i]);
		}
	}
	for (uint i = 0; i < cannons.size(); ++i) {
		if (cannons[i].isInsideViewFrustrum(frustum))
			renderVectors.objects[cannons[i].meshId].push_back(&cannons[i]);
	}
	for (uint i = 0; i < cannonRow.size(); ++i) {
		if (cannonRow[i].isInsideViewFrustrum(frustum)) {
			renderVectors.texturedObjects[cannonRow[i].texture].push_back(&cannonRow[i]);
		}
	}
}

ShipWall::ShipWall()
{
}


ShipWall::~ShipWall()
{
}
