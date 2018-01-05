#pragma once
#include "ObjectContainer.h"

struct FloorParams;

class ShipWall : public ObjectContainer {
	std::vector<TexturedObject> wall;
	std::vector<Object> cannons;
public:
	void clear();
	void init(const Assets& assets, const FloorParams& fp, glm::vec3 floorPos, float wallHeight, glm::uint rows);
	void groupDrawableObjects(const FrustumG& frustum, RenderVectors& renderVectors) override;
	ShipWall();
	~ShipWall();
};

