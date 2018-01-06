#include "Coin.h"
#include "Game.h"

void Coin::staticStart() {
	soundManager = Game::instance().getSoundManager();
	getCoinSound = soundManager->loadSound("sounds/Effect_coin_collect.wav", FMOD_DEFAULT);
}

void Coin::start() {
	startPoint = origin->getY();
	up = true;
}

void Coin::update(int deltaTime) {
	if (player->getX() == origin->getX() &&
		player->getZ() == origin->getZ()) {
		Game::instance().coins += 1;
		
		soundManager->playSound(getCoinSound);
		origin->destroy();
		return;
	}
	origin->rotateY(rotationSpeed);
	if (up) {
		origin->moveY(verticalSpeed);
		if (origin->getY() > startPoint + maxHeight) {
			origin->setY(startPoint + maxHeight);
			up = false;
		}
	}
	else {
		origin->moveY(-verticalSpeed);
		if (origin->getY() < startPoint) {
			origin->setY(startPoint);
			up = true;
		}
	}
}

Coin::Coin() : player(NULL)
{
}


Coin::~Coin()
{
}

const float Coin::rotationSpeed = 0.1f;
const float Coin::verticalSpeed = 0.05f;
const float Coin::maxHeight = 0.6f;
const SoundManager* Coin::soundManager;
FMOD::Sound* Coin::getCoinSound;