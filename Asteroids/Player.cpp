#include "Player.h"
#include "Utils.h"
#include <iostream>
using namespace std;
Player::Player(Texture* texture, Texture* thrusterTexture, SoundBuffer thrusterSound)
{
	this->thrusterSound = thrusterSound;
	invulTimer = 0;
	lives = PLAYER_LIVES;
	drawThruster = false;
	shape.setRadius(PLAYER_SIZE);
	shape.setOrigin(PLAYER_SIZE , PLAYER_SIZE);
	shape.setPosition(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);	
	shape.setTexture(texture);
	velocity = (Vector2f(0, 0));
	facing = (getDirectionVectorFromDegrees(shape.getRotation()));
	thruster.setOrigin(THRUSTER_SIZE.x / 2, THRUSTER_SIZE.y / 2);
	thruster.setPosition(shape.getPosition()- facing*THRUSTER_OFFSET);	
	thruster.setTexture(thrusterTexture);
	thruster.setSize(THRUSTER_SIZE);
}

void Player::collide() {
	if (invulTimer <= 0) {
		--lives;	
		invulTimer = INVUL_COOLDOWN;
		shape.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		velocity = (Vector2f(0, 0));
		shape.setRotation(0);
	}
}

int Player::getType() {
	return GOPLAYER;
}

Vector2f Player::getPosition() {
	if (invulTimer > 0) {
		return Vector2f(-1000, -1000);
	}
	return shape.getPosition();
}

int Player::getRotation() {
	return shape.getRotation();
}

float Player::getSpeed() {
	return vectorMagnitude(velocity);
}

Vector2f Player::getLaserPosition() {
	return shape.getPosition();
}

void Player::update(float dt) {
	if (invulTimer > 0) {
		shape.setFillColor(Color::Blue);
		shape.setOutlineColor(Color::Blue);
		shape.setOutlineThickness(2);
		invulTimer -= dt;
	}
	else {
		shape.setFillColor(Color::White);
		shape.setOutlineColor(Color::White);
		shape.setOutlineThickness(0);
	}
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
		if (sound.getStatus() != sound.Playing) {
			sound.setVolume(50);
			sound.setBuffer(thrusterSound);
			sound.play();
		}
		drawThruster = true;
		Vector2f newVelocity = velocity + facing * dt * ACCELERATION;	
		if (vectorMagnitude(newVelocity) <= MAXSPEED) {
			velocity = newVelocity;
		}
	}
	else {
		drawThruster = false;
		sound.stop();
	}

	if (vectorMagnitude(velocity) > 0) {
		Vector2f drag = normalizeVector(velocity) * dt * DRAG_MULTIPLIER;
		velocity -= drag;
	}
	Vector2f position = shape.getPosition();
	position += velocity;
	shape.setPosition(wrapAround(position, PLAYER_SIZE, PLAYER_SIZE));
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
