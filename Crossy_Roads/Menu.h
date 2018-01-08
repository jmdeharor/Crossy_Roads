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
	MouseControl,
	KeyboardControl,
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
	const SoundManager* soundManager;
	FMOD::Sound* clickSound;

private:
	bool initiated;
	bool click;
	Sprite* menuLogo;
	Sprite* mouseControl, *keyBoardControl;
	Sprite* menuShop;
	Texture menuLogoTexture;
	Texture mouseControlPressedTexture, mouseControlTexture;
	Texture keyboardControlPressedTexture, keyboardControlTexture;
	Texture menuShopTexture;
	Texture menuShopPressedTexture;
	ShaderProgram shaderProgram;
};


#endif // _SCENE_INCLUDE