#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "Menu.h"
#include "Game.h"
#include <cmath>
#include <GL/glut.h>
using namespace glm;


Menu::Menu() : initiated(false)
{
	menuLogo = NULL;
	menuHighScore = NULL;
}


Menu::~Menu()
{
	if (menuLogo != NULL)
		delete menuLogo;
	if (menuHighScore != NULL)
		delete menuHighScore;
}

const int keyFramesPerSec = 5;
const int jumpingFrames = 5;

void Menu::firstInit() {
	initShaders();
	initTextures();
	menuLogo = Sprite::createSprite(vec2(694, 294), vec2(1), &menuLogoTexture, &shaderProgram);
	menuHighScore = Sprite::createSprite(vec2(94,94), vec2(1), &menuHighScoreTexture, &shaderProgram);
	menuShop = Sprite::createSprite(vec2(94, 94), vec2(1), &menuShopTexture, &shaderProgram);


}


void Menu::init() {
	if (!initiated) {
		initiated = true;
		firstInit();
	}
	click = false;
	menuLogo->setPosition(vec2(SCREEN_WIDTH / (float)2, SCREEN_HEIGHT / (float)2));
	menuHighScore->setPosition(vec2(SCREEN_WIDTH - 100, SCREEN_HEIGHT - 100));
	menuShop->setPosition(vec2(100, SCREEN_HEIGHT - 100));
}

bool highScoreButton(int x, int y) {
	return x >= SCREEN_WIDTH - (100 + 47 + 10) && x <= SCREEN_WIDTH - (100 - 47 - 10) && y >= SCREEN_HEIGHT - (100 + 47 + 10) && y <= SCREEN_HEIGHT - (100 - 47 - 10);
}

bool shopButton(int x, int y) {
	return x >= (100 - 47 - 10) && x <= (100 + 47 + 10) && y >= SCREEN_HEIGHT - (100 + 47 + 10) && y <= SCREEN_HEIGHT - (100 - 47 - 10);
}

void Menu::performClickAction(int x, int y) {
	if (highScoreButton(x, y)) {
		//Open HighScore
	}
	else if (shopButton(x, y)) {
		//Open Shop
		Game::instance().setCurrentState(GameState::SHOP);
	}
	else {
		//Disable menu
		Game::instance().setCurrentState(GameState::PLAYING);
	}
}

void Menu::update(int deltaTime) {
	currentTime += deltaTime;
	if (Game::instance().getCurrentState() == GameState::MENU) {
		if (click && !Game::instance().getLeftButtonPressed()) {
			performClickAction(Game::instance().getXPressed(), Game::instance().getYPressed());
			click = false;
		}
		if (Game::instance().getLeftButtonPressed()) click = true;
		int x = Game::instance().getX();
		int y = Game::instance().getY();
		if (highScoreButton(x, y))
			menuHighScore->setTexture(&menuHighScorePressedTexture);
		else if (shopButton(x, y))
			menuShop->setTexture(&menuShopPressedTexture);
		else {
			menuHighScore->setTexture(&menuHighScoreTexture);
			menuShop->setTexture(&menuShopTexture);
		}
	}

}

void Menu::render() {
	glm::mat4 modelview;
	shaderProgram.use();
	glm::mat4 projection = ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	shaderProgram.setUniformMatrix4f("projection", projection);
	shaderProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	shaderProgram.setUniformMatrix4f("modelview", modelview);
	shaderProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	menuLogo->render();
	menuHighScore->render();
	menuShop->render();
}

void Menu::initTextures() {
	menuLogoTexture.loadFromFile("images/logo.png", TEXTURE_PIXEL_FORMAT_RGBA, true);
	menuLogoTexture.wrapS = GL_CLAMP_TO_EDGE;
	menuLogoTexture.wrapT = GL_CLAMP_TO_EDGE;
	menuLogoTexture.minFilter = GL_NEAREST;
	menuLogoTexture.magFilter = GL_NEAREST;

	menuHighScoreTexture.loadFromFile("images/button_highscores.png", TEXTURE_PIXEL_FORMAT_RGBA, true);
	menuHighScoreTexture.wrapS = GL_CLAMP_TO_EDGE;
	menuHighScoreTexture.wrapT = GL_CLAMP_TO_EDGE;
	menuHighScoreTexture.minFilter = GL_NEAREST;
	menuHighScoreTexture.magFilter = GL_NEAREST;

	menuHighScorePressedTexture.loadFromFile("images/button_highscores_pressed.png", TEXTURE_PIXEL_FORMAT_RGBA, true);
	menuHighScorePressedTexture.wrapS = GL_CLAMP_TO_EDGE;
	menuHighScorePressedTexture.wrapT = GL_CLAMP_TO_EDGE;
	menuHighScorePressedTexture.minFilter = GL_NEAREST;
	menuHighScorePressedTexture.magFilter = GL_NEAREST;

	menuShopTexture.loadFromFile("images/button_shop.png", TEXTURE_PIXEL_FORMAT_RGBA, true);
	menuShopTexture.wrapS = GL_CLAMP_TO_EDGE;
	menuShopTexture.wrapT = GL_CLAMP_TO_EDGE;
	menuShopTexture.minFilter = GL_NEAREST;
	menuShopTexture.magFilter = GL_NEAREST;

	menuShopPressedTexture.loadFromFile("images/button_shop_pressed.png", TEXTURE_PIXEL_FORMAT_RGBA, true);
	menuShopPressedTexture.wrapS = GL_CLAMP_TO_EDGE;
	menuShopPressedTexture.wrapT = GL_CLAMP_TO_EDGE;
	menuShopPressedTexture.minFilter = GL_NEAREST;
	menuShopPressedTexture.magFilter = GL_NEAREST;
}

void Menu::initShaders() {
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