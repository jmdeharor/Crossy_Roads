#pragma once
#include <vector>
#include "TexturedObject.h"

class ObjectContainer {
public:
	virtual void groupDrawableObjects(
		std::vector<std::vector<Object*>>& objects,
		std::vector<std::vector<TexturedObject*>>& texturedObjects,
		glm::mat4& viewProjection
	) = 0;
	ObjectContainer();
	~ObjectContainer();
};

