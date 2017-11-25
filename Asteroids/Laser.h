#pragma once
#include "GameObject.h"
class Laser :
	public GameObject
{
public:
	void update(float dt);
	void draw(RenderWindow* window);
	Vector2f getPosition();
	FloatRect getCollider();
	int getType();
	Laser(Vector2f position, Vector2f facing);
	~Laser();
private:
	RectangleShape shape;
};

