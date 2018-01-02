#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include <iostream>

void Game::init() {
	bPlay = true;
	glClearColor(0.f, 1.f, 1.f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glClearStencil(0);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	scene.init();
	menu.init();
	currentState = GameState::MENU;
}

GameState Game::getCurrentState() {
	return currentState;
}

void Game::setCurrentState(GameState newState) {
	currentState = newState;
}

bool Game::update(int deltaTime) {
	soundManager.update();
	scene.update(deltaTime);
	menu.update(deltaTime);
	
	return bPlay;
}

void Game::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	scene.render();
	if(currentState == GameState::MENU)
		menu.render();
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





