#include "Player.h"
#include "Utils.h"
#include <iostream>
using namespace std;
Player::Player()
{
	drawThruster = false;
	shape.setPosition(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
	shape.setFillColor(Color::Blue);
	shape.setSize(PLAYER_SIZE);
	shape.setOrigin(PLAYER_SIZE.x / 2, PLAYER_SIZE.y / 2);
	setVelocity(Vector2f(0, 0));
	setFacing(getDirectionVectorFromDegrees(shape.getRotation()));
	thruster.setOrigin(THRUSTER_SIZE.x / 2, THRUSTER_SIZE.y / 2);
	thruster.setPosition(shape.getPosition()- facing*THRUSTER_OFFSET);
	thruster.setFillColor(Color::Red);
	thruster.setSize(THRUSTER_SIZE);
}

Vector2f Player::getPosition() {
	return shape.getPosition();
}

FloatRect Player::getCollider() {
	return shape.getGlobalBounds();
}

void Player::update(float dt) {
	float rotation;
	if (Keyboard::isKeyPressed(Keyboard::Left)){
		rotation = shape.getRotation();
		rotation -= dt *TURNSPEED;
		shape.setRotation(rotation);
	}
	if (Keyboard::isKeyPressed(Keyboard::Right)) {
		rotation = shape.getRotation();
		rotation += dt * TURNSPEED;
		shape.setRotation(rotation);
	}
	rotation = shape.getRotation();
	facing = getDirectionVectorFromDegrees(rotation);

	if (Keyboard::isKeyPressed(Keyboard::Up)) {
		drawThruster = true;
		Vector2f newVelocity = velocity + facing * dt * ACCELERATION;		
		if (vectorMagnitude(newVelocity) <= MAXSPEED) {
			velocity = newVelocity;
		}
	}
	else {
		drawThruster = false;
	}

	if (vectorMagnitude(velocity) > 0) {
		Vector2f drag = normalizeVector(velocity) * dt * DRAG_MULTIPLIER;
		velocity -= drag;
	}
	Vector2f position = getPosition();
	position += velocity;
	if (position.x > SCREEN_WIDTH) {
		position.x = 0;
	}
	if (position.x < 0) {
		position.x = SCREEN_WIDTH;
	}
	if (position.y > SCREEN_HEIGHT) {
		position.y = 0;
	}
	if (position.y < 0) {
		position.y = SCREEN_HEIGHT;
	}

	shape.setPosition(position);
	thruster.setPosition(shape.getPosition() - facing*THRUSTER_OFFSET);
	thruster.setRotation(shape.getRotation());

}
void Player::draw(RenderWindow* window) {
	window->draw(shape);
	if (drawThruster) {
		window->draw(thruster);
	}
}


Player::~Player()
{
}
