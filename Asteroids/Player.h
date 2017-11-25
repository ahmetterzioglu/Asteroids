#pragma once
#include "GameObject.h"
class Player :
	public GameObject
{
public:
	void update(float dt);
	void draw(RenderWindow* window);
	Vector2f getPosition();
	FloatRect getCollider();
	Player();
	~Player();
private:
	RectangleShape shape;
	RectangleShape thruster;
	bool drawThruster;
};

