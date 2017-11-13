#pragma once
#include "RenderizableObject.h"
#include "UpdatableObject.h"

class GameObject : public RenderizableObject, public UpdatableObject {
	void firstInit();
	bool initiated;
public:
	GameObject();
	virtual ~GameObject();
	void init();
};