#include "Player.h"
#include "Utils.h"

using namespace std;
Player::Player(Texture* texture, Texture* thrusterTexture)
{
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
	shape.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	--lives;
	if (lives == 0) {
		toBeDestroyed = true;
	}
}

int Player::getType() {
	return GOPLAYER;
}

Vector2f Player::getPosition() {
	return shape.getPosition();
}

FloatRect Player::getCollider() {
	return shape.getGlobalBounds();
}

int Player::getRotation() {
	return shape.getRotation();
}

float Player::getSpeed() {
	return vectorMagnitude(velocity);
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
	shape.setPosition(wrapAround(position));
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
