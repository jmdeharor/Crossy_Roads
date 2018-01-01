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
	menu.mouseMove(x, y);
	std::cout << "Position: " << x << " " << "y" << endl;
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
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

const SoundManager * Game::getSoundManager() const {
	return &soundManager;
}





