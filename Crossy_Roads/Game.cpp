#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include <iostream>
using namespace glm;

void Game::init() {
	//TODO: Initialize coins from file
	coins = 10000000;
	initShaders();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	bPlay = true;
	glClearColor(0.f, 1.f, 1.f, 1.0f);
	glClearStencil(0);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	scene.init();
	menu.init();
	shop.init();
	endGameView.init();
	scene.playerControl = false;
	currentState = GameState::MENU;
	mouseCursorTexture.loadFromFile("images/cursor.png", TEXTURE_PIXEL_FORMAT_RGBA, true);
	mouseCursorTexture.wrapS = GL_CLAMP_TO_EDGE;
	mouseCursorTexture.wrapT = GL_CLAMP_TO_EDGE;
	mouseCursorTexture.minFilter = GL_NEAREST;
	mouseCursorTexture.magFilter = GL_NEAREST;
	mouseCursor = Sprite::createSprite(vec2(32, 32), vec2(1), &mouseCursorTexture, &shaderProgram);
}

GameState Game::getCurrentState() {
	return currentState;
}

bool Game::update(int deltaTime) {
	mouseCursor->setPosition(vec2(x, y));
	soundManager.update();

	SceneReturn sceneRet;
	MenuReturn menuRet;
	EndGameViewReturn endRet;
	ShopReturn shopRet;
	switch (currentState) {
	case GameState::PLAYING:
		sceneRet = scene.update(deltaTime);
		if (sceneRet == SceneReturn::EndGame) {
			scene.playerControl = false;
			currentState = GameState::ENDGAME;
		}
		else if (getSpecialKey(GLUT_KEY_F1)) {
			scene.playerControl = false;
			currentState = GameState::MENU;
		}
		break;
	case GameState::MENU:
		scene.update(deltaTime);
		menuRet = menu.update(deltaTime);
		switch (menuRet) {
		case MenuReturn::Exit:
			scene.playerControl = true;
			currentState = GameState::PLAYING;
			break;
		case MenuReturn::Shop:
			currentState = GameState::SHOP;
			break;
		}
		break;
	case GameState::SHOP:
		shopRet = shop.update(deltaTime);
		if (shopRet == ShopReturn::Menu) {
			currentState = GameState::MENU;
		}
		break;
	case GameState::ENDGAME:
		scene.update(deltaTime);
		endRet = endGameView.update(deltaTime);
		if (endRet == EndGameViewReturn::BackToMenu) {
			currentState = GameState::MENU;
		}
		break;
	}
	return bPlay;
}

void Game::render() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	switch (currentState) {
	case GameState::PLAYING:
		glEnable(GL_DEPTH_TEST);
		scene.render();
		glDisable(GL_DEPTH_TEST);
		break;
	case GameState::MENU:
		glEnable(GL_DEPTH_TEST);
		scene.render();
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		menu.render();
		glDisable(GL_BLEND);
		break;
	case GameState::SHOP:
		glEnable(GL_BLEND);
		shop.render();
		glDisable(GL_BLEND);
		break;
	case GameState::ENDGAME:
		glEnable(GL_DEPTH_TEST);
		scene.render();
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		endGameView.render();
		glDisable(GL_BLEND);
		break;
	}
	glEnable(GL_BLEND);
	mat4 modelview;
	shaderProgram.use();
	mat4 projection = ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	shaderProgram.setUniformMatrix4f("projection", projection);
	shaderProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	mouseCursor->render();
	glDisable(GL_BLEND);
}

void Game::keyPressed(int key)
{
	if(key == 27) // Escape code
		bPlay = false;
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
	this->x = x;
	this->y = y;
}

void Game::mousePress(int button, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && !mouseLeftPressed) {
		xPressed = x;
		yPressed = y;
		mouseLeftPressed = true;
	}
}

void Game::mouseRelease(int button, int x, int y)
{
	mouseLeftPressed = false;
}

bool Game::getLeftButtonPressed() {
	return mouseLeftPressed;
}

void Game::windowResize(int w, int h) {
	glViewport(0, 0, w, h);
	scene.resize(w, h);
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}

int Game::getX() {
	return x;
}

int Game::getY() {
	return y;
}

int Game::getXPressed() {
	return xPressed;
}

int Game::getYPressed() {
	return yPressed;
}

const SoundManager * Game::getSoundManager() const {
	return &soundManager;
}

int Game::getCoins() {
	return coins;
}

void Game::setCoins(int newCoins) {
	coins = newCoins;
}

void Game::initShaders() {
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





