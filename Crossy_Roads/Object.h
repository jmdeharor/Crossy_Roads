#pragma once
#include "Mesh.h"

class Object {
protected:
	const Mesh* mesh;
	glm::vec3 center;
	glm::mat4 model;
	glm::vec3 pos;
	glm::vec3 rot;
	glm::vec3 scale;
	bool modified;

public:
	string name;
	void setCenter(glm::vec3 newCenter);
	void setCenterToBaseCenter();
	void updateModel();
	void update(int deltaTime);
	void setPos(glm::vec3 pos);
	void move(float x, float y, float z);
	void move(glm::vec3 movement);
	void rotateX(float angle);
	void rotateY(float angle);
	void rotateZ(float angle);
	void setRotationX(float angle);
	void setRotationY(float angle);
	void setRotationZ(float angle);
	void setScale(glm::vec3 scale);
	const glm::mat4* getModel();
	virtual void render(ShaderProgram& program);
	Object();
	void setMesh(const Mesh* mesh);
	virtual ~Object();
	glm::vec3 getPos() const;
	float getHeight() const;
};

