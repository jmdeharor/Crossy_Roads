#ifndef _SHOP_INCLUDE
#define _SHOP_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Sprite.h"
#include "Texture.h"
#include "SoundManager.h"
#include "Text.h"


class Shop {

public:
	Shop();
	~Shop();

	void init();
	void render();
	void update(int deltatime);


private:
	void firstInit();
	void initTextures();
	void initShaders();
	void initTexts();
	void performClickAction(int x, int y);
	bool isButton(Sprite* sprite, int x, int y);

private:
	bool initiated;
	bool click;

	const SoundManager* soundManager;
	FMOD::Sound* clickSound;

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
	Texture backArrowTex;
	Texture shopBackgroundTexture;
	Texture char1Tex, char2Tex, char3Tex, char4Tex, char5Tex, char6Tex, char7Tex, char8Tex;
	Texture char1TexLock, char2TexLock, char3TexLock, char4TexLock;
	Texture char5TexLock, char6TexLock, char7TexLock, char8TexLock;
	Texture char1TexLockHL, char2TexLockHL, char3TexLockHL, char4TexLockHL;
	Texture char5TexLockHL, char6TexLockHL, char7TexLockHL, char8TexLockHL;
	Text char1Price, char2Price, char3Price, char4Price;
	Text char5Price, char6Price, char7Price, char8Price;
	bool char1Locked, char2Locked, char3Locked, char4Locked, char5Locked, char6Locked, char7Locked, char8Locked;

	ShaderProgram shaderProgram;
	float currentTime;



};


#endif // _SCENE_INCLUDE