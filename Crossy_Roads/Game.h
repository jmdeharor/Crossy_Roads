#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE

#include <glm/glm.hpp>
#include "Scene.h"
#include "Menu.h"
#include "Shop.h"
#include "SoundManager.h"
#include "Sprite.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include "EndGameView.h"


#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

// Game is a singleton (a class with a single instance) that represents our whole application


enum class GameState {
	MENU,
	PLAYING,
	ENDGAME,
	SHOP
};

class Game
{

public:
	Game() {}
	
	
	static Game &instance()
	{
		static Game G;
	
		return G;
	}
	
	void init();
	bool update(int deltaTime);
	void render();
	
	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button, int x, int y);
	void mouseRelease(int button, int x, int y);
	void windowResize(int w, int h);
	int getX();
	int getY();
	int getXPressed();
	int getYPressed();
	bool getKey(int key) const;
	bool getSpecialKey(int key) const;
	bool getLeftButtonPressed();
	bool getMouseControl() const;
	int getCoins();
	void setCoins(int newCoins);

	GameState getCurrentState();

	const SoundManager* getSoundManager() const;

public:
	int coins;
	int charSelected;
private:
	bool mouseControl;
	SoundManager soundManager;
	bool bPlay;                       // Continue to play game?
	Scene scene;                      // Scene to render
	Menu menu;
	Shop shop;
	EndGameView endGameView;
	bool keys[256], specialKeys[256]; // Store key states so that 
	                                  // we can have access at any time
	int x, y, xPressed, yPressed, xReleased, yReleased;
	bool mouseLeftPressed;
	GameState currentState;
	Sprite* mouseCursor;
	Texture mouseCursorTexture;
	ShaderProgram shaderProgram;
	void initShaders();
	bool resetPressed;

};


#endif // _GAME_INCLUDE


