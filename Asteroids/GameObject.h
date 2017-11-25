#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;
class GameObject
{
public:
	virtual void update(float dt) = 0;
	virtual void draw(RenderWindow* window) = 0;
	virtual FloatRect getCollider() = 0;
	virtual Vector2f getPosition() = 0;

	virtual Vector2f getFacing();
	virtual void setFacing(Vector2f facing);
	virtual Vector2f getVelocity();
	virtual void setVelocity(Vector2f velocity);
	virtual float getSpeed();
	virtual void setSpeed(float speed);
	GameObject();
	~GameObject();
protected:
	Vector2f facing;
	Vector2f velocity;
	float speed;


};

