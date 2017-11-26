#pragma once
#include "GameObject.h"
#include "Utils.h"
class Asteroid :
	public GameObject
{
public:
	void update(float dt);
	void draw(RenderWindow* window);
	void collide();
	Vector2f getPosition();
	FloatRect getCollider();
	int getType();
	Asteroid(Texture* texture, Vector2f position, int lives, int level);
	~Asteroid();
private:
	CircleShape shape;
	int level;
};

