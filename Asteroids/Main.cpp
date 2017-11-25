#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include <iostream>
#include "Utils.h"
#include "Player.h"
#include "Laser.h"
using namespace sf;
using namespace std;

void getEvents();
void updateState(vector<GameObject*> gameObjects, vector<GameObject*>* buckets[BUCKET_COLS][BUCKET_ROWS], float dt);
void renderFrame(vector<GameObject*> gameObjects);
void bucket_remove(Vector2i b, GameObject* obj, vector<GameObject*>* buckets[BUCKET_COLS][BUCKET_ROWS]);
void bucket_add(Vector2i b, GameObject* obj, vector<GameObject*>* buckets[BUCKET_COLS][BUCKET_ROWS]);
Laser* createLaser(Player* player);

RenderWindow window;

int main()
{
	window.create(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Asteroids");
	Clock clock;
	Texture playerTexture;
	float laserTimer = 0;
	if (!playerTexture.loadFromFile("player.png"))
	{
		cout << "texture1 Image couldn't load!";
	}
	Texture thrusterTexture;
	if (!thrusterTexture.loadFromFile("thruster.png"))
	{
		cout << "thruster Image couldn't load!";
	}
	vector<GameObject*>* buckets[BUCKET_COLS][BUCKET_ROWS];
	for (int i = 0; i < BUCKET_COLS; i++) {
		for (int j = 0; j < BUCKET_ROWS; j++) {
			buckets[i][j] = new vector<GameObject*>;
		}
	}
	vector<GameObject*> gameObjects;
	Player* player = new Player(&playerTexture, &thrusterTexture);
	gameObjects.push_back(player);
	bucket_add(getBucket(player->getPosition()), player, buckets);


	Font font;
	if (!font.loadFromFile("sansation.ttf"))
	{
		cout << "Font couldn't load!";
	}


	///////////////////////////////////////////////////////
	Text uiText;
	uiText.setCharacterSize(20);
	uiText.setFont(font);
	uiText.setPosition((SCREEN_WIDTH / 2) - 200, SCREEN_HEIGHT - 30);
	uiText.setFillColor(Color::White);
	uiText.setString(to_string(getBucket(player->getPosition()).x)+" "+ to_string(getBucket(player->getPosition()).y));
	//////////////////////////////////////////////////

	while (window.isOpen())
	{
		getEvents();
		float dt = clock.restart().asSeconds();
		laserTimer += dt;
		if (Keyboard::isKeyPressed(Keyboard::Space) && laserTimer >= LASER_COOLDOWN) {
			laserTimer = 0;
			Laser* laser = createLaser(player);
			gameObjects.push_back(laser);
			bucket_add(getBucket(laser->getPosition()), laser, buckets);
		}
		updateState(gameObjects, buckets, dt);
		renderFrame(gameObjects);
		//////////////////////////////////////
		uiText.setString(to_string(laserTimer) + " Bucket: "+to_string(getBucket(player->getPosition()).x) + " " + to_string(getBucket(player->getPosition()).y));
		window.draw(uiText);
		///////////////////////////////////////

		window.display();
	}

	return 0;
}

Laser* createLaser(Player* player) {
	Laser* laser = new Laser(player->getPosition() + player->getFacing() * LASER_OFFSET, player->getFacing(), player->getRotation());
	return laser;
}

void updateState(vector<GameObject*> gameObjects, vector<GameObject*>* buckets[BUCKET_COLS][BUCKET_ROWS], float dt) {
	for each(GameObject* gameObject in gameObjects)
	{
		Vector2i curBucket = getBucket(gameObject->getPosition());
		gameObject->update(dt);
		Vector2i newBucket = getBucket(gameObject->getPosition());
		if (curBucket != newBucket)
		{
			bucket_remove(curBucket, gameObject, buckets);
			bucket_add(newBucket, gameObject, buckets);
		}
		//detect_collisions(obj, newBucket);
	}

}

void bucket_add(Vector2i bucket, GameObject* obj, vector<GameObject*>* buckets[BUCKET_COLS][BUCKET_ROWS])
{
	vector<GameObject*> & target = *buckets[bucket.x][bucket.y];
	target.push_back(obj);
}

void bucket_remove(Vector2i bucket,	GameObject* obj, vector<GameObject*>* buckets[BUCKET_COLS][BUCKET_ROWS])
{
	vector<GameObject*> & target= *buckets[bucket.x][bucket.y];

	for (int i = 0; i < target.size(); ++i)
	{
		if (target[i] == obj)
		{
			target.erase(target.begin() + i);
			break;
		}
	}
}



void renderFrame(vector<GameObject*> gameObjects) {
	window.clear(Color(0, 0, 0));
	for each(GameObject* go in gameObjects) {
		go->draw(&window);
	}
}

void getEvents() {
	Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();
	}
}
