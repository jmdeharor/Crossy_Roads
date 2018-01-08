#include "EndGameView.h"
#include <iostream>
#include "Game.h"
using namespace std;
using namespace glm;


void EndGameView::firstInit() {
	game = &Game::instance();
	initShaders();
	buttonReturnTexture.loadFromFile("images/white.png", TEXTURE_PIXEL_FORMAT_RGBA, false);
	buttonReturnTexture.minFilter = GL_NEAREST;
	buttonReturnTexture.magFilter = GL_NEAREST;
	buttonReturnTexture.applyParams();

	soundManager = game->getSoundManager();
	clickSound = soundManager->loadSound("sounds/Effect_click.wav", FMOD_DEFAULT);

	message.init("fonts/treamd.ttf");
	buttonText.init("fonts/treamd.ttf");
	buttonReturn = Sprite::createSprite(vec2(200, 50), vec2(1), &buttonReturnTexture, &shaderProgram);
}

void EndGameView::init() {
	GameObject::init();

	buttonReturn->setPosition(vec2(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f + 90));
	buttonPressed = true;
	buttonOpacity = 0.6f;
}

EndGameViewReturn EndGameView::update(int deltaTime) {
	if (buttonReturn->inside(game->getX(), game->getY())) {
		buttonOpacity = 0.9f;
	}
	else {
		buttonOpacity = 0.6f;
	}
	if (game->getLeftButtonPressed()) {
		buttonPressed = true;
	}
	else if (buttonPressed) {
		buttonPressed = false;
		if (buttonReturn->inside(game->getXPressed(), game->getYPressed())) {
			soundManager->playSound(clickSound);
			return EndGameViewReturn::BackToMenu;
		}
	}
	return EndGameViewReturn::Nothing;
}

void EndGameView::render() {
	shaderProgram.use();
	mat4 projection = ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	shaderProgram.setUniformMatrix4f("projection", projection);
	shaderProgram.setUniform4f("color", 0, 0, 0, buttonOpacity);

	buttonReturn->render();
	vec2 messageSize = message.getQuadSize("You are dead!", 64);
	message.render("You are dead!", vec2(SCREEN_WIDTH/2.f - messageSize.x/2, SCREEN_HEIGHT/2.f - messageSize.y/2 - 10), 64, vec4(1,0,0,1));
	messageSize = buttonText.getQuadSize("Return to menu", 32);
	buttonText.render("Return to menu", vec2(SCREEN_WIDTH / 2.f - messageSize.x / 2, SCREEN_HEIGHT / 2.f - messageSize.y / 2 + 100), 32, vec4(1, 0, 0, 1));
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
