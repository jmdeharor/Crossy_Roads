#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "Shop.h"
#include "Game.h"
#include <cmath>
#include <GL/glut.h>
#include "SoundManager.h"
#include <fstream>

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
	clickSound = soundManager->loadSound("sounds/Effect_click.wav", FMOD_DEFAULT);
	charTexs.resize(8);
	charTexsLocked.resize(8);
	charTexsLockedHL.resize(8);
	locked.resize(8, true);
	ifstream reader ("data/shop_info.txt",ifstream::in);
	for (int i = 0; i < 8; ++i) {
		int aux;
		reader >> aux;
		locked[i] = (bool)aux;
	}
	reader.close();
	chars.resize(8);
	charTexts.resize(8);
	prices.resize(8);
	initShaders();
	initTextures();
	initTexts();
	shopBackground = Sprite::createSprite(vec2(1024, 768), vec2(1), &shopBackgroundTexture, &shaderProgram);
	for (int i = 0; i < 8; ++i) {
		if(locked[i])
			chars[i] = Sprite::createSprite(vec2(125, 125), vec2(1), &charTexsLocked[i], &shaderProgram);
		else
			chars[i] = Sprite::createSprite(vec2(125, 125), vec2(1), &charTexs[i], &shaderProgram);
	}
	backArrow = Sprite::createSprite(vec2(80, 60), vec2(1), &backArrowTex, &shaderProgram);
}


void Shop::init() {
	if (!initiated) {
		initiated = true;
		firstInit();
	}
	click = false;
	shopBackground->setPosition(vec2(SCREEN_WIDTH / (float)2, SCREEN_HEIGHT / (float)2));
	chars[0]->setPosition(vec2(SCREEN_WIDTH / (float)5 * 1, SCREEN_HEIGHT / (float)3 * 1));
	chars[1]->setPosition(vec2(SCREEN_WIDTH / (float)5 * 2, SCREEN_HEIGHT / (float)3 * 1));
	chars[2]->setPosition(vec2(SCREEN_WIDTH / (float)5 * 3, SCREEN_HEIGHT / (float)3 * 1));
	chars[3]->setPosition(vec2(SCREEN_WIDTH / (float)5 * 4, SCREEN_HEIGHT / (float)3 * 1));
	chars[4]->setPosition(vec2(SCREEN_WIDTH / (float)5 * 1, SCREEN_HEIGHT / (float)3 * 2));
	chars[5]->setPosition(vec2(SCREEN_WIDTH / (float)5 * 2, SCREEN_HEIGHT / (float)3 * 2));
	chars[6]->setPosition(vec2(SCREEN_WIDTH / (float)5 * 3, SCREEN_HEIGHT / (float)3 * 2));
	chars[7]->setPosition(vec2(SCREEN_WIDTH / (float)5 * 4, SCREEN_HEIGHT / (float)3 * 2));
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
	for (int i = 0; i < 8; ++i) {
		if (isButton(chars[i], x, y) && locked[i]) {
			if (atoi(prices[i].c_str()) <= Game::instance().getCoins()) {
				chars[i]->setTexture(&charTexs[i]);
				locked[i] = false;
				Game::instance().setCoins(Game::instance().getCoins() - atoi(prices[i].c_str()));
				prices[i] = "Hired";
				ofstream writer("data/shop_info.txt", ofstream::out);
				for (int i = 0; i < 8; ++i) {
					writer << (int)locked[i] << " ";
				}
				writer.close();
				ofstream writer2("data/coin_info.txt", ofstream::out);
				writer2 << Game::instance().getCoins();
				writer2.close();

				//play unlock sound
			}
			else {
				//play no money sound and display message
			}

		}
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
		bool onButton = false;
		for (int i = 0; i < 8; ++i) {
			if (isButton(chars[i], x, y) && locked[i]) {
				chars[i]->setTexture(&charTexsLockedHL[i]);
				onButton = true;
			}
		}
		if (!onButton) {
			for (int i = 0; i < 8; ++i) {
				if (locked[i]) chars[i]->setTexture(&charTexsLocked[i]);
			}
		}
	}

}

void Shop::render() {
	glm::mat4 modelview;
	shaderProgram.use();
	glm::mat4 projection = ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	shaderProgram.setUniformMatrix4f("projection", projection);
	shaderProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	shaderProgram.setUniformMatrix4f("modelview", modelview);
	shaderProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	shopBackground->render();
	for (int i = 0; i < 8; ++i) {
		chars[i]->render();
	}
	backArrow->render();
	charTexts[0].render(prices[0], vec2(chars[0]->getPosition().x - 48, chars[0]->getPosition().y + chars[0]->getSize().y), 64, vec4(0, 0, 0, 1));
	charTexts[1].render(prices[1], vec2(chars[1]->getPosition().x - 48, chars[1]->getPosition().y + chars[1]->getSize().y), 64, vec4(0, 0, 0, 1));
	charTexts[2].render(prices[2], vec2(chars[2]->getPosition().x - 48, chars[2]->getPosition().y + chars[2]->getSize().y), 64, vec4(0, 0, 0, 1));
	charTexts[3].render(prices[3], vec2(chars[3]->getPosition().x - 48, chars[3]->getPosition().y + chars[3]->getSize().y), 64, vec4(0, 0, 0, 1));
	charTexts[4].render(prices[4], vec2(chars[4]->getPosition().x - 48, chars[4]->getPosition().y + chars[4]->getSize().y), 64, vec4(0, 0, 0, 1));
	charTexts[5].render(prices[5], vec2(chars[5]->getPosition().x - 48, chars[5]->getPosition().y + chars[5]->getSize().y), 64, vec4(0, 0, 0, 1));
	charTexts[6].render(prices[6], vec2(chars[6]->getPosition().x - 48, chars[6]->getPosition().y + chars[6]->getSize().y), 64, vec4(0, 0, 0, 1));
	charTexts[7].render(prices[7], vec2(chars[7]->getPosition().x - 48, chars[7]->getPosition().y + chars[7]->getSize().y), 64, vec4(0, 0, 0, 1));
}

void initTexture(Texture& texture, string path) {
	texture.loadFromFile(path, TEXTURE_PIXEL_FORMAT_RGBA, true);
	texture.wrapS = GL_CLAMP_TO_EDGE;
	texture.wrapT = GL_CLAMP_TO_EDGE;
	texture.minFilter = GL_NEAREST;
	texture.magFilter = GL_NEAREST;
}

void Shop::initTextures() {
	initTexture(shopBackgroundTexture, "images/shop_2.png");
	for (int i = 0; i < 8; ++i) {
		initTexture(charTexsLocked[i], "images/char_locked_" + to_string(i) + ".png");
		initTexture(charTexs[i], "images/char_unlocked_" + to_string(i) + ".png");
		initTexture(charTexsLockedHL[i], "images/char_locked_highlighted_" + to_string(i) +".png");
	}
	initTexture(backArrowTex, "images/back_arrow.png");
}
void Shop::initTexts() {
	for (int i = 0; i < 8; ++i) {
		charTexts[i].init("fonts/treamd.ttf");
		if (locked[i])
			prices[i] = to_string((i + 1) * 100);
		else
			prices[i] = "Hired";
	}
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
