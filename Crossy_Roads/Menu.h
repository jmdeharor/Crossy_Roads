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


private:
	void firstInit();
	void initTextures();
	void initShaders();
	void performClickAction(int x, int y);

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