#pragma once
#include "Text.h"
#include "Sprite.h"
#include "GameObject.h"

class Game;

enum class EndGameViewReturn {
	BackToMenu,
	Nothing
};

class EndGameView : GameObject {
	Game* game;
	Text scoreText;
	Texture messageTexture;
	Texture buttonSaveTexture;
	Texture buttonReturnTexture;
	Sprite* message;
	Sprite* buttonSave;
	Sprite* buttonReturn;
	ShaderProgram shaderProgram;
	bool buttonPressed;

	void initShaders();
public:
	void firstInit() override;
	void init() override;
	EndGameViewReturn update(int deltaTime);
	void render();
	EndGameView();
	~EndGameView();
};

