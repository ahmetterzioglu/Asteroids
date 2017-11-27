#include "Explosion.h"



Explosion::Explosion(Texture* texture, Vector2f position, int size)
{
	Vector2f actualSize = EXPLOSION_SIZE * (float)size;
	shape.setSize(actualSize);
	shape.setOrigin(actualSize.x/2, actualSize.y/2);
	shape.setPosition(position);
	shape.setTexture(texture);
	timer = 0.2;
}

void Explosion::update(float dt) {
	if (timer > 0) {
		timer -= dt;
	}
	else {
		toBeDestroyed = true;
	}
}

void Explosion::draw(RenderWindow* window) {
	window->draw(shape);
}

int Explosion::getType() {
	return GOIGNORE;
}

void Explosion::collide() {

}

Vector2f Explosion::getPosition() {
	return shape.getPosition();
}

Explosion::~Explosion()
{
}
