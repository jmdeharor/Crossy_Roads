#pragma once
#include "ObjectContainer.h"

struct FloorParams;

class ShipWall : public ObjectContainer {
	std::vector<TexturedObject> wall;
	std::vector<TexturedObject> cannonRow;
	std::vector<Object> cannons;
	glm::uint half;
public:
	void clear();
	void init(const Assets& assets, const FloorParams& fp, glm::vec3 floorPos, float wallHeight, glm::uint rows, glm::uint cannonSeparation);
	void groupDrawableObjects(const FrustumG& frustum, RenderVectors& renderVectors) override;
	ShipWall();
	~ShipWall();
};

