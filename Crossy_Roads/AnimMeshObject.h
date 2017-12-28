#pragma once
#include "Object.h"

class AnimMeshObject : public Object {
	glm::uint size;
	float timeAnimation;
	float millisecsPerKeyframe;
	glm::uint currentKeyframe;
public:
	void update(int deltatime);
	void setMeshes(glm::uint size, float millisecsPerKeyframe);
	IdMesh getMesh() const;
	AnimMeshObject();
	~AnimMeshObject();
};

