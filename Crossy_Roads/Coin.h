#pragma once
#include "MonoBehaviour.h"

class Coin : public MonoBehaviour {
	float startPoint;
	bool up;
	static const SoundManager* soundManager;
	static FMOD::Sound* getCoinSound;
public:
	const Object* player;
	static const float rotationSpeed;
	static const float verticalSpeed;
	static const float maxHeight;
	static void staticStart();
	void start() override;
	void update(int deltaTime) override;
	Coin();
	~Coin();
};

