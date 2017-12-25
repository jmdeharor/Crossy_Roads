#pragma once
#include <fmod_studio.hpp>
#include <string>

class SoundManager {
	FMOD::Studio::System* system;
	FMOD::System* lowLevelSystem;
public:
	FMOD::Sound* loadSound(const std::string& file, bool loop);
	FMOD::Channel* playSound(FMOD::Sound* sound);
	void update();
	SoundManager();
	~SoundManager();
};

