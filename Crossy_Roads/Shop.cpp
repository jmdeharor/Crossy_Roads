#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "Shop.h"
#include "Game.h"
#include <cmath>
#include <GL/glut.h>
#include "SoundManager.h"

using namespace glm;


Shop::Shop() : initiated(false)
{
	shopBackground = NULL;
	char1 = NULL;
}


Shop::~Shop()
{
	if (shopBackground != NULL)
		delete shopBackground;
	if (char1 != NULL)
		delete char1;

}

const int keyFramesPerSec = 5;
const int jumpingFrames = 5;

void Shop::firstInit() {

	soundManager = Game::instance().getSoundManager();
	clickSound = soundManager->loadSound("sounds/Effect_click.wav", false);

	initShaders();
	initTextures();
	initTexts();
	shopBackground = Sprite::createSprite(vec2(1024, 768), vec2(1), &shopBackgroundTexture, &shaderProgram);
	char1 = Sprite::createSprite(vec2(125, 125), vec2(1), &char1TexLock, &shaderProgram);
	char2 = Sprite::createSprite(vec2(125, 125), vec2(1), &char2TexLock, &shaderProgram);
	char3 = Sprite::createSprite(vec2(125, 125), vec2(1), &char3TexLock, &shaderProgram);
	char4 = Sprite::createSprite(vec2(125, 125), vec2(1), &char4TexLock, &shaderProgram);
	char5 = Sprite::createSprite(vec2(125, 125), vec2(1), &char5TexLock, &shaderProgram);
	char6 = Sprite::createSprite(vec2(125, 125), vec2(1), &char6TexLock, &shaderProgram);
	char7 = Sprite::createSprite(vec2(125, 125), vec2(1), &char7TexLock, &shaderProgram);
	char8 = Sprite::createSprite(vec2(125, 125), vec2(1), &char8TexLock, &shaderProgram);
	backArrow = Sprite::createSprite(vec2(80, 60), vec2(1), &backArrowTex, &shaderProgram);
}


void Shop::init() {
	if (!initiated) {
		initiated = true;
		firstInit();
	}
	char1Locked = true;
	char2Locked = true;
	char3Locked = true;
	char4Locked = true;
	char5Locked = true;
	char6Locked = true;
	char7Locked = true;
	char8Locked = true;
	click = false;
	shopBackground->setPosition(vec2(SCREEN_WIDTH / (float)2, SCREEN_HEIGHT / (float)2));
	char1->setPosition(vec2(SCREEN_WIDTH / (float)5 * 1, SCREEN_HEIGHT / (float)3 * 1));
	char2->setPosition(vec2(SCREEN_WIDTH / (float)5 * 2, SCREEN_HEIGHT / (float)3 * 1));
	char3->setPosition(vec2(SCREEN_WIDTH / (float)5 * 3, SCREEN_HEIGHT / (float)3 * 1));
	char4->setPosition(vec2(SCREEN_WIDTH / (float)5 * 4, SCREEN_HEIGHT / (float)3 * 1));
	char5->setPosition(vec2(SCREEN_WIDTH / (float)5 * 1, SCREEN_HEIGHT / (float)3 * 2));
	char6->setPosition(vec2(SCREEN_WIDTH / (float)5 * 2, SCREEN_HEIGHT / (float)3 * 2));
	char7->setPosition(vec2(SCREEN_WIDTH / (float)5 * 3, SCREEN_HEIGHT / (float)3 * 2));
	char8->setPosition(vec2(SCREEN_WIDTH / (float)5 * 4, SCREEN_HEIGHT / (float)3 * 2));
	backArrow->setPosition(vec2(SCREEN_WIDTH - 100, 100));

}

bool Shop::isButton(Sprite* sprite, int x, int y) {
	vec2 size = sprite->getSize();
	vec2 pos = sprite->getPosition();
	int xmin = pos.x - size.x / 2.f;
	int xmax = pos.x + size.x / 2.f;
	int ymin = pos.y - size.y / 2.f;
	int ymax = pos.y + size.y / 2.f;
	return x >= xmin && x <= xmax && y >= ymin && y <= ymax;
}

void Shop::performClickAction(int x, int y) {
	if (isButton(backArrow, x, y)) {
		FMOD::Channel* channel = soundManager->playSound(clickSound);
		channel->setVolume(2);
		Game::instance().setCurrentState(GameState::MENU);
	}
	if (isButton(char1, x, y)) {
		char1->setTexture(&char1Tex);
		char1Locked = false;
	}
	if (isButton(char2, x, y)) {
		char2->setTexture(&char2Tex);
		char2Locked = false;
	}
	if (isButton(char3, x, y)) {
		char3->setTexture(&char3Tex);
		char3Locked = false;
	}
	if (isButton(char4, x, y)) {
		char4->setTexture(&char4Tex);
		char4Locked = false;
	}
	if (isButton(char5, x, y)) {
		char5->setTexture(&char5Tex);
		char5Locked = false;
	}
	if (isButton(char6, x, y)) {
		char6->setTexture(&char6Tex);
		char6Locked = false;
	}
	if (isButton(char7, x, y)) {
		char7->setTexture(&char7Tex);
		char7Locked = false;
	}
	if (isButton(char8, x, y)) {
		char8->setTexture(&char8Tex);
		char8Locked = false;
	}
}

void Shop::update(int deltaTime) {
	currentTime += deltaTime;
	if (Game::instance().getCurrentState() == GameState::SHOP) {
		if (click && !Game::instance().getLeftButtonPressed()) {
			performClickAction(Game::instance().getXPressed(), Game::instance().getYPressed());
			click = false;
		}
		if (Game::instance().getLeftButtonPressed()) click = true;
		int x = Game::instance().getX();
		int y = Game::instance().getY();
		if (isButton(char1, x, y) && char1Locked)
			char1->setTexture(&char1TexLockHL);
		else if (isButton(char2, x, y) && char2Locked)
			char2->setTexture(&char2TexLockHL);
		else if (isButton(char3, x, y) && char3Locked)
			char3->setTexture(&char3TexLockHL);
		else if (isButton(char4, x, y) && char4Locked)
			char4->setTexture(&char4TexLockHL);
		else if (isButton(char5, x, y) && char5Locked)
			char5->setTexture(&char5TexLockHL);
		else if (isButton(char6, x, y) && char6Locked)
			char6->setTexture(&char6TexLockHL);
		else if (isButton(char7, x, y) && char7Locked)
			char7->setTexture(&char7TexLockHL);
		else if (isButton(char8, x, y) && char8Locked)
			char8->setTexture(&char8TexLockHL);
		else {
			if (char1Locked) char1->setTexture(&char1TexLock);
			if (char2Locked) char2->setTexture(&char2TexLock);
			if (char3Locked) char3->setTexture(&char3TexLock);
			if (char4Locked) char4->setTexture(&char4TexLock);
			if (char5Locked) char5->setTexture(&char5TexLock);
			if (char6Locked) char6->setTexture(&char6TexLock);
			if (char7Locked) char7->setTexture(&char7TexLock);
			if (char8Locked) char8->setTexture(&char8TexLock);
		}


	}

}

void Shop::render() {
	glEnable(GL_BLEND);
	glm::mat4 modelview;
	shaderProgram.use();
	glm::mat4 projection = ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	shaderProgram.setUniformMatrix4f("projection", projection);
	shaderProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	shaderProgram.setUniformMatrix4f("modelview", modelview);
	shaderProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	//char1Price.render("abc", char1->getPosition(), 64, vec4(1, 1, 1, 1));
	shopBackground->render();
	char1->render();
	char2->render();
	char3->render();
	char4->render();
	char5->render();
	char6->render();
	char7->render();
	char8->render();
	backArrow->render();
	char1Price.render("100", vec2(char1->getPosition().x - 48, char1->getPosition().y + char1->getSize().y), 64, vec4(0, 0, 0, 1));
	char2Price.render("200", vec2(char2->getPosition().x - 48, char2->getPosition().y + char2->getSize().y), 64, vec4(0, 0, 0, 1));
	char3Price.render("300", vec2(char3->getPosition().x - 48, char3->getPosition().y + char3->getSize().y), 64, vec4(0, 0, 0, 1));
	char4Price.render("400", vec2(char4->getPosition().x - 48, char4->getPosition().y + char4->getSize().y), 64, vec4(0, 0, 0, 1));
	char5Price.render("500", vec2(char5->getPosition().x - 48, char5->getPosition().y + char5->getSize().y), 64, vec4(0, 0, 0, 1));
	char6Price.render("600", vec2(char6->getPosition().x - 48, char6->getPosition().y + char6->getSize().y), 64, vec4(0, 0, 0, 1));
	char7Price.render("700", vec2(char7->getPosition().x - 48, char7->getPosition().y + char7->getSize().y), 64, vec4(0, 0, 0, 1));
	char8Price.render("800", vec2(char8->getPosition().x - 48, char8->getPosition().y + char8->getSize().y), 64, vec4(0, 0, 0, 1));
	glDisable(GL_BLEND);
}

void initTexture(Texture& texture, string path) {
	texture.loadFromFile(path, TEXTURE_PIXEL_FORMAT_RGBA, true);
	texture.setWrapS(GL_CLAMP_TO_EDGE);
	texture.setWrapT(GL_CLAMP_TO_EDGE);
	texture.setMinFilter(GL_NEAREST);
	texture.setMagFilter(GL_NEAREST);
}

void Shop::initTextures() {
	initTexture(shopBackgroundTexture, "images/shop_2.png");
	initTexture(char1TexLock, "images/placeholder_locked.png");
	initTexture(char2TexLock, "images/placeholder_locked.png");
	initTexture(char3TexLock, "images/placeholder_locked.png");
	initTexture(char4TexLock, "images/placeholder_locked.png");
	initTexture(char5TexLock, "images/placeholder_locked.png");
	initTexture(char6TexLock, "images/placeholder_locked.png");
	initTexture(char7TexLock, "images/placeholder_locked.png");
	initTexture(char8TexLock, "images/placeholder_locked.png");
	initTexture(char1Tex, "images/placeholder_unlocked.png");
	initTexture(char2Tex, "images/placeholder_unlocked.png");
	initTexture(char3Tex, "images/placeholder_unlocked.png");
	initTexture(char4Tex, "images/placeholder_unlocked.png");
	initTexture(char5Tex, "images/placeholder_unlocked.png");
	initTexture(char6Tex, "images/placeholder_unlocked.png");
	initTexture(char7Tex, "images/placeholder_unlocked.png");
	initTexture(char8Tex, "images/placeholder_unlocked.png");
	initTexture(char1TexLockHL, "images/placeholder_locked_highlighted.png");
	initTexture(char2TexLockHL, "images/placeholder_locked_highlighted.png");
	initTexture(char3TexLockHL, "images/placeholder_locked_highlighted.png");
	initTexture(char4TexLockHL, "images/placeholder_locked_highlighted.png");
	initTexture(char5TexLockHL, "images/placeholder_locked_highlighted.png");
	initTexture(char6TexLockHL, "images/placeholder_locked_highlighted.png");
	initTexture(char7TexLockHL, "images/placeholder_locked_highlighted.png");
	initTexture(char8TexLockHL, "images/placeholder_locked_highlighted.png");
	initTexture(backArrowTex, "images/back_arrow.png");
}

void Shop::initTexts() {
	char1Price.init("fonts/treamd.ttf");
	char2Price.init("fonts/treamd.ttf");
	char3Price.init("fonts/treamd.ttf");
	char4Price.init("fonts/treamd.ttf");
	char5Price.init("fonts/treamd.ttf");
	char6Price.init("fonts/treamd.ttf");
	char7Price.init("fonts/treamd.ttf");
	char8Price.init("fonts/treamd.ttf");

}

void Shop::initShaders() {
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture2.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture2.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	shaderProgram.init();
	shaderProgram.addShader(vShader);
	shaderProgram.addShader(fShader);
	shaderProgram.link();
	if (!shaderProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << shaderProgram.log() << endl << endl;
	}
	shaderProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}
