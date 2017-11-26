#include "Laser.h"
#include "Utils.h"


Laser::Laser(Vector2f position, Vector2f facing, int rotation)
{
	
	shape.setFillColor(Color::Cyan);
	shape.setSize(LASER_SIZE);
	shape.setOrigin(LASER_SIZE.x / 2, LASER_SIZE.y / 2);
	shape.setPosition(position);
	shape.setRotation(rotation);
	shape.setOutlineColor(Color::White);
	shape.setOutlineThickness(1);
	velocity = (Vector2f(0, 0));
	this->facing = facing;
	timer = LASER_TIMER;
}

void Laser::collide() {
	toBeDestroyed = true;
}

void Laser::draw(RenderWindow* window) {
	window->draw(shape);
}

int Laser::getType() {
	return GOLASER;
}

Vector2f Laser::getPosition() {
	return shape.getPosition();
}

FloatRect Laser::getCollider() {
	return shape.getGlobalBounds();
}

void Laser::update(float dt) {
	timer -= dt;
	Vector2f position = getPosition();
	position += facing * LASER_SPEED * dt;
	shape.setPosition(wrapAround(position));
	if (timer <= 0) {
		toBeDestroyed = true;
	}
}


Laser::~Laser()
{
}
