#pragma once
#include "GameObject.h"
#include <SFML/Audio.hpp>
class Player :
	public GameObject
{
public:
	void update(float dt);
	void draw(RenderWindow* window);
	void collide();
	Vector2f getPosition();
	int getRotation();
	float getSpeed();
	int getType();
	Vector2f getLaserPosition();
	Player(Texture* texture, Texture* thrusterTexture, SoundBuffer thrusterSound);
	~Player();
private:
	CircleShape shape;
	RectangleShape thruster;
	bool drawThruster;
	float invulTimer;
	Sound sound;
	SoundBuffer thrusterSound;
};

