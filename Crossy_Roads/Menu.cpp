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
}


void Menu::init() {
	if (!initiated) {
		initiated = true;
		firstInit();
	}
	else
		menuLogo->setTexture(&menuLogoTexture);
	menuLogo->setPosition(vec2(SCREEN_WIDTH / (float)2, SCREEN_HEIGHT / (float)2));
	menuHighScore->setPosition(vec2(SCREEN_WIDTH - 100, SCREEN_HEIGHT - 100));
}

void Menu::update(int deltaTime) {
	currentTime += deltaTime;
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
}

void Menu::initTextures() {
	menuLogoTexture.loadFromFile("images/logo.png", TEXTURE_PIXEL_FORMAT_RGBA, true);
	menuLogoTexture.setWrapS(GL_CLAMP_TO_EDGE);
	menuLogoTexture.setWrapT(GL_CLAMP_TO_EDGE);
	menuLogoTexture.setMinFilter(GL_NEAREST);
	menuLogoTexture.setMagFilter(GL_NEAREST);

	menuHighScoreTexture.loadFromFile("images/button_highscores.png", TEXTURE_PIXEL_FORMAT_RGBA, true);
	menuHighScoreTexture.setWrapS(GL_CLAMP_TO_EDGE);
	menuHighScoreTexture.setWrapT(GL_CLAMP_TO_EDGE);
	menuHighScoreTexture.setMinFilter(GL_NEAREST);
	menuHighScoreTexture.setMagFilter(GL_NEAREST);

	menuHighScorePressedTexture.loadFromFile("images/button_highscores_pressed.png", TEXTURE_PIXEL_FORMAT_RGBA, true);
	menuHighScorePressedTexture.setWrapS(GL_CLAMP_TO_EDGE);
	menuHighScorePressedTexture.setWrapT(GL_CLAMP_TO_EDGE);
	menuHighScorePressedTexture.setMinFilter(GL_NEAREST);
	menuHighScorePressedTexture.setMagFilter(GL_NEAREST);
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

void Menu::keyPressed(int key)
{
}

void Menu::keyReleased(int key)
{
}

void Menu::specialKeyPressed(int key)
{
}

void Menu::specialKeyReleased(int key)
{
}

void Menu::mouseMove(int x, int y)
{
	if (x >= SCREEN_WIDTH - (100 + 47 + 10) && x <= SCREEN_WIDTH - (100 - 47 - 10) && y >= SCREEN_HEIGHT - (100 + 47 + 10) && y <= SCREEN_HEIGHT - (100 - 47 - 10))
		menuHighScore->setTexture(&menuHighScorePressedTexture);
	else
		menuHighScore->setTexture(&menuHighScoreTexture);
}

void Menu::mousePress(int button)
{
}

void Menu::mouseRelease(int button)
{
}
