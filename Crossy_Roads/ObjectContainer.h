#pragma once
#include <vector>
#include "TexturedObject.h"
#include "FrustumG.h"

class ObjectContainer {
public:
	virtual void groupDrawableObjects(
		std::vector<std::vector<Object*>>& objects,
		std::vector<std::vector<TexturedObject*>>& texturedObjects,
		const FrustumG& frustum
	) = 0;
	ObjectContainer();
	~ObjectContainer();
};

