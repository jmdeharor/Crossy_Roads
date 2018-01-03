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


#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

// Game is a singleton (a class with a single instance) that represents our whole application


enum class GameState {
	MENU,
	PLAYING,
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
	int getCoins();
	void setCoins(int newCoins);

	GameState getCurrentState();
	void setCurrentState(GameState newState);

	const SoundManager* getSoundManager() const;

private:
	SoundManager soundManager;
	bool bPlay;                       // Continue to play game?
	Scene scene;                      // Scene to render
	Menu menu;
	Shop shop;
	bool keys[256], specialKeys[256]; // Store key states so that 
	                                  // we can have access at any time
	int coins;
	int x, y, xPressed, yPressed, xReleased, yReleased;
	bool mouseLeftPressed;
	GameState currentState;
	Sprite* mouseCursor;
	Texture mouseCursorTexture;
	ShaderProgram shaderProgram;
	void initShaders();


};


#endif // _GAME_INCLUDE


