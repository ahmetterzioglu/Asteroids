#include "Asteroid.h"



Asteroid::Asteroid(Texture* texture, Vector2f position, int lives, int level)
{
	this->level = level;
	this->lives = lives;
	shape.setRadius(ASTEROID_SIZE * lives);
	shape.setOrigin(ASTEROID_SIZE* lives, ASTEROID_SIZE* lives);
	shape.setPosition(position);
	shape.setTexture(texture);
	shape.setOutlineColor(Color::White);
	shape.setOutlineThickness(1);
	float x = -1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2));
	float y = -1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2));
	velocity = normalizeVector( Vector2f(x,y));
}

void Asteroid::update(float dt) {
	float speed = ASTEROID_SPEED + 10 * level;
	Vector2f position = getPosition();
	position += velocity * speed * dt;
	shape.setPosition(wrapAround(position));
	
	float rotation = shape.getRotation();
	rotation -= dt *TURNSPEED /5;
	shape.setRotation(rotation);

}

void Asteroid::draw(RenderWindow* window) {
	window->draw(shape);
}

void Asteroid::collide() {
	toBeDestroyed = true;
}

int Asteroid::getType() {
	return GOASTEROID;
}

Vector2f Asteroid::getPosition() {
	return shape.getPosition();
}

FloatRect Asteroid::getCollider() {
	return shape.getGlobalBounds();
}

Asteroid::~Asteroid()
{
}
