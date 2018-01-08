#pragma once
#include "Text.h"
#include "Sprite.h"
#include "SoundManager.h"
#include "GameObject.h"

class Game;

enum class EndGameViewReturn {
	BackToMenu,
	Nothing
};

class EndGameView : GameObject {
	Game* game;
	const SoundManager* soundManager;
	FMOD::Sound* clickSound;
	Texture buttonReturnTexture;
	Text message;
	Text buttonText;
	Sprite* buttonReturn;
	ShaderProgram shaderProgram;
	bool buttonPressed;
	float buttonOpacity;

	void initShaders();
public:
	void firstInit() override;
	void init() override;
	EndGameViewReturn update(int deltaTime);
	void render();
	EndGameView();
	~EndGameView();
};

