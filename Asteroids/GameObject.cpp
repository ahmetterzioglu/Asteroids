#include "GameObject.h"
using namespace sf;


GameObject::GameObject()
{
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


GameObject::~GameObject()
{
}
