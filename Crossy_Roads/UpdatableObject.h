#pragma once
class UpdatableObject {
public:
	virtual void update(int deltaTime) = 0;
	UpdatableObject();
	virtual ~UpdatableObject();
};

