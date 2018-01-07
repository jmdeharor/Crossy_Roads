#pragma once
#include "MonoBehaviour.h"
#include "ParticleSystem.h"

class Coin : public MonoBehaviour {
	float startPoint;
	bool up;
	static const SoundManager* soundManager;
	static FMOD::Sound* getCoinSound;
	static ParticleSystem* particleSystem;
public:
	const Object* player;
	static const float rotationSpeed;
	static const float verticalSpeed;
	static const float maxHeight;
	static void staticStart(ParticleSystem* particleSystem);
	void start() override;
	void update(int deltaTime) override;
	Coin();
	~Coin();
};

