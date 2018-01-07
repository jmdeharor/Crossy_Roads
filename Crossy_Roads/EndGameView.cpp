#include "EndGameView.h"
#include <iostream>
#include "Game.h"
using namespace std;
using namespace glm;


void EndGameView::firstInit() {
	game = &Game::instance();
	initShaders();
	messageTexture.loadFromFile("images/grass_plane.png", TEXTURE_PIXEL_FORMAT_RGBA, false);
	messageTexture.minFilter = GL_NEAREST;
	messageTexture.magFilter = GL_NEAREST;
	messageTexture.applyParams();
	buttonReturnTexture.loadFromFile("images/grass_plane.png", TEXTURE_PIXEL_FORMAT_RGBA, false);
	buttonReturnTexture.minFilter = GL_NEAREST;
	buttonReturnTexture.magFilter = GL_NEAREST;
	buttonReturnTexture.applyParams();
	buttonSaveTexture.loadFromFile("images/grass_plane.png", TEXTURE_PIXEL_FORMAT_RGBA, false);
	buttonSaveTexture.minFilter = GL_NEAREST;
	buttonSaveTexture.magFilter = GL_NEAREST;
	buttonSaveTexture.applyParams();

	message = Sprite::createSprite(vec2(200, 100), vec2(1), &messageTexture, &shaderProgram);
	buttonReturn = Sprite::createSprite(vec2(60, 50), vec2(1), &buttonReturnTexture, &shaderProgram);
	buttonSave = Sprite::createSprite(vec2(60, 50), vec2(1), &buttonReturnTexture, &shaderProgram);
}

void EndGameView::init() {
	GameObject::init();

	message->setPosition(vec2(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f));
	buttonReturn->setPosition(vec2(SCREEN_WIDTH / 2.f - 100, SCREEN_HEIGHT / 2.f + 200));
	buttonSave->setPosition(vec2(SCREEN_WIDTH / 2.f + 100, SCREEN_HEIGHT / 2.f + 200));
	buttonPressed = true;
}

EndGameViewReturn EndGameView::update(int deltaTime) {
	if (game->getLeftButtonPressed()) {
		buttonPressed = true;
	}
	else if (buttonPressed) {
		buttonPressed = false;
		if (buttonReturn->inside(game->getXPressed(), game->getYPressed())) {
			return EndGameViewReturn::BackToMenu;
		}
	}
	return EndGameViewReturn::Nothing;
}

void EndGameView::render() {
	shaderProgram.use();
	mat4 projection = ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	shaderProgram.setUniformMatrix4f("projection", projection);
	shaderProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	
	message->render();
	buttonSave->render();
	buttonReturn->render();
}

EndGameView::EndGameView()
{
}


EndGameView::~EndGameView()
{
}

void EndGameView::initShaders() {
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
