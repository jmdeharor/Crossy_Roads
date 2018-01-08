#ifndef _SHOP_INCLUDE
#define _SHOP_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Sprite.h"
#include "Texture.h"
#include "SoundManager.h"
#include "Text.h"

enum class ShopReturn {
	Menu,
	Nothing
};

class Shop {

public:
	Shop();
	~Shop();

	void init();
	void render();
	ShopReturn update(int deltatime);


private:
	void firstInit();
	void initTextures();
	void initShaders();
	void initTexts();
	ShopReturn performClickAction(int x, int y);
	void initSounds();
	bool isButton(Sprite* sprite, int x, int y);

private:
	bool initiated;
	bool click;

	const SoundManager* soundManager;
	FMOD::Sound* clickSound;

	vector<FMOD::Sound*> unlockSounds;
	Sprite* shopBackground;
	Sprite* char1;
	Sprite* char2;
	Sprite* char3;
	Sprite* char4;
	Sprite* char5;
	Sprite* char6;
	Sprite* char7;
	Sprite* char8;
	Sprite* backArrow;
	Sprite* selection;
	Texture selectionTexture;
	Texture backArrowTex;
	Texture shopBackgroundTexture;
	std::vector<Texture> charTexs;
	std::vector<Texture> charTexsLocked;
	std::vector<Texture> charTexsLockedHL;
	std::vector<bool> locked;
	std::vector<Sprite*> chars;
	std::vector<Text> charTexts;
	std::vector<string> prices;
	Text noCoins;
	bool noCoinsBool;

	ShaderProgram shaderProgram;
};


#endif // _SCENE_INCLUDE