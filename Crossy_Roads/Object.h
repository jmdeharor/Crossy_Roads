#pragma once
#include "GameObject.h"
#include "Mesh.h"

class Object : public GameObject {
protected:
	Mesh* mesh;
	glm::vec3 center;
	glm::mat4 model;
	glm::vec3 pos;
	glm::vec3 rot;
	glm::vec3 scale;
public:
	void update(int deltaTime);
	void setPos(glm::vec3 pos);
	void setScale(glm::vec3 scale);
	const glm::mat4* getModel() const;
	void updateModel();
	void render();
	Object();
	void setMesh(Mesh* mesh);
	virtual ~Object();
};

