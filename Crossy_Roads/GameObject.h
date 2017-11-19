#pragma once

class GameObject {
protected:
	virtual void firstInit();
	bool initiated;
public:
	GameObject();
	virtual ~GameObject();
	virtual void init();
};