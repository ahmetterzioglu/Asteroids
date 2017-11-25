#include "GameObject.h"
using namespace sf;


GameObject::GameObject()
{
}



Vector2f GameObject::getFacing() {
	return facing;
}
void GameObject::setFacing(Vector2f facing) {
	this->facing = facing;
}
	
Vector2f GameObject::getVelocity() {
	return velocity;
}
void GameObject::setVelocity(Vector2f velocity) {
	this->velocity = velocity;
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
