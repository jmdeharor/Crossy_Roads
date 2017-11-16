#pragma once
#include "RenderizableObject.h"
#include "UpdatableObject.h"

class GameObject : public RenderizableObject, public UpdatableObject {
protected:
	virtual void firstInit();
	bool initiated;
public:
	GameObject();
	virtual ~GameObject();
	virtual void init();
};