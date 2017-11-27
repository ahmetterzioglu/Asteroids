#pragma once
#include "GameObject.h"
#include "Utils.h"
class Explosion :
	public GameObject
{
public:
	void update(float dt);
	void draw(RenderWindow* window);
	void collide();
	Vector2f getPosition();
	int getType();
	Explosion(Texture* texture, Vector2f position, int size);
	~Explosion();
private:
	RectangleShape shape;
	float timer;
};

