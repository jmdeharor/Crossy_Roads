#pragma once
#include <vector>
#include "FrustumG.h"
#include "TexturedObject.h"
#include "ShadowedObject.h"

struct RenderVectors {
	std::vector<std::vector<Object*>> objects;
	std::vector<std::vector<Object*>> texturedObjects;
	std::vector<std::vector<Object*>> shadowMapOpjects;
	std::vector<std::vector<ShadowedObject*>> projectionShadowObjects;
};

class ObjectContainer {
public:
	virtual void groupDrawableObjects(
		const FrustumG& frustum, RenderVectors& renderVectors
	) = 0;
	ObjectContainer();
	~ObjectContainer();
};

