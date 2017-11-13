#pragma once
class GameObject {
	void firstInit();
	bool initiated;
public:
	GameObject();
	~GameObject();
	void init();
	virtual void update(int deltaTime) = 0;
	virtual void render() = 0;
};