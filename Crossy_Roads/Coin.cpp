#include "Coin.h"

void Coin::start() {
	startPoint = origin->getY();
	up = true;
}

void Coin::update(int deltaTime) {
	origin->rotateY(0.1f);
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

Coin::Coin()
{
}


Coin::~Coin()
{
}

const float Coin::rotationSpeed = 0.05f;
const float Coin::verticalSpeed = 0.05f;
const float Coin::maxHeight = 0.6f;