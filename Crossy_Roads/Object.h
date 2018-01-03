#pragma once
#include "Mesh.h"
#include "Assets.h"
#include "FrustumG.h"

class Object {
protected:
	glm::vec3 center;
	glm::mat4 model;
	glm::vec3 pos;
	glm::vec3 rot;
	glm::vec3 scale;
	bool modified;

public:
	IdMesh meshId;
	const Mesh* mesh;

public:
	bool isInsideViewFrustrum(const FrustumG& frustum);
	bool collidesWith(Object& otherObject);
	glm::uint getTriangles() const;
	string name;
	void setCenter(glm::vec3 newCenter);
	void setCenterToBaseCenter();
	void updateModel();
	void setPos(glm::vec3 pos);
	void setX(float x);
	void setY(float y);
	void setZ(float z);
	void move(float x, float y, float z);
	void moveX(float x);
	void moveY(float y);
	void moveZ(float z);
	void move(glm::vec3 movement);
	void rotateX(float angle);
	void rotateY(float angle);
	void rotateZ(float angle);
	void setRotationX(float angle);
	void setRotationY(float angle);
	void setRotationZ(float angle);
	void setScale(glm::vec3 scale);
	const glm::mat4* getModel();
	void setMesh(glm::uint meshId, const Mesh* mesh);
	void setMesh(const Mesh* mesh);
	glm::vec3 getPos() const;
	float getX() const;
	float getY() const;
	float getZ() const;
	float getHeight() const;
	glm::vec2 getHorizontalPlanePos() const;
	glm::vec3 getRotation() const;
	glm::vec3 getMin();
	glm::vec3 getMax();

	Object();
	virtual ~Object();
};

