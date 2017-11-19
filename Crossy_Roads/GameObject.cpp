#include "GameObject.h"

void GameObject::firstInit() {
}

GameObject::GameObject(): initiated(false)
{
}

GameObject::~GameObject()
{
}

void GameObject::init() {
	if (!initiated) {
		initiated = true;
		firstInit();
	}
}