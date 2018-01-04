#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Sprite.h"
#include "Texture.h"
#include "SoundManager.h"

enum class MenuReturn {
	Exit,
	Shop,
	Nothing
};


class Menu {

public:
	Menu();
	~Menu();

	void init();
	void render();
	MenuReturn update(int deltatime);


private:
	void firstInit();
	void initTextures();
	void initShaders();
	MenuReturn performClickAction(int x, int y);

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
};


#endif // _SCENE_INCLUDE