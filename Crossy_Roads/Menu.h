#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Sprite.h"
#include "Texture.h"
#include "SoundManager.h"


class Menu {

public:
	Menu();
	~Menu();

	void init();
	void render();
	void update(int deltatime);
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);

private:
	void firstInit();
	void initTextures();
	void initShaders();

private:
	bool initiated;
	bool click;
	Sprite* menuLogo;
	Sprite* menuHighScore;
	Sprite* menuShop;
	Texture menuLogoTexture;
	Texture menuHighScoreTexture;
	Texture menuHighScorePressedTexture;
	Texture menuShopTexture;
	Texture menuShopPressedTexture;
	ShaderProgram shaderProgram;
	float currentTime;


	
};


#endif // _SCENE_INCLUDE