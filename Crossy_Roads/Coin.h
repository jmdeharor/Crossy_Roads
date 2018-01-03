#pragma once
#include "MonoBehaviour.h"
class Coin : public MonoBehaviour {
	float startPoint;
	bool up;
public:
	static const float rotationSpeed;
	static const float verticalSpeed;
	static const float maxHeight;
	void start() override;
	void update(int deltaTime) override;
	Coin();
	~Coin();
};

