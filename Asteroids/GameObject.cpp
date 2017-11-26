#include "GameObject.h"
using namespace sf;


GameObject::GameObject()
{
}

void GameObject::setVelocity(Vector2f veclocity) {
	this->velocity = veclocity;
}

Vector2f GameObject::getFacing() {
	return facing;
}

	
Vector2f GameObject::getVelocity() {
	return velocity;
}

float GameObject::getSpeed(){
	return speed;
}
void GameObject::setSpeed(float speed) {
	this->speed = speed;
}

bool GameObject::isToBeDestroyed() {
	return toBeDestroyed;
}

int GameObject::getLives() {
	return lives;
}

GameObject::~GameObject()
{
}
