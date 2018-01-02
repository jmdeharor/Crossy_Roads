#pragma once
#include <fmod_studio.hpp>
#include <string>

class SoundManager {
	FMOD::Studio::System* system;
	FMOD::System* lowLevelSystem;
public:
	FMOD::Sound* loadSound(const std::string& file, FMOD_MODE mode) const;
	FMOD::Channel* playSound(FMOD::Sound* sound) const;
	void update();
	SoundManager();
	~SoundManager();
};

