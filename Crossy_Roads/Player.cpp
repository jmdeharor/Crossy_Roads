#include "Player.h"

void Player::firstInit() {
	playerMesh.loadFromFile("");
}

void Player::jump() {
}

void Player::init() {
	GameObject::init();
}

void Player::update(int deltaTime) {

}

void Player::render(ShaderProgram & program) {
	playerObject.render(program);
}

void Player::renderShadow(ShaderProgram & program) {
	playerObject.renderShadow(program);
}

Player::Player(){
}

Player::~Player()
{
}
