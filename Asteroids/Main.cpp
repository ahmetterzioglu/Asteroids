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
using namespace sf;
using namespace std;

void getEvents();
void updateState(vector<GameObject*> gameObjects, vector<GameObject*>* buckets[BUCKET_COLS][BUCKET_ROWS], float dt);
void renderFrame(Player player);
void bucket_remove(Vector2i b, GameObject* obj, vector<GameObject*>* buckets[BUCKET_COLS][BUCKET_ROWS]);
void bucket_add(Vector2i b, GameObject* obj, vector<GameObject*>* buckets[BUCKET_COLS][BUCKET_ROWS]);


RenderWindow window;

int main()
{
	window.create(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Asteroids");
	Clock clock;
	Texture playerTexture;
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
		updateState(gameObjects, buckets, dt);
		renderFrame(*player);
		//////////////////////////////////////
		uiText.setString(to_string(getBucket(player->getPosition()).x) + " " + to_string(getBucket(player->getPosition()).y));
		window.draw(uiText);
		///////////////////////////////////////

		window.display();
	}

	return 0;
}

void updateState(vector<GameObject*> gameObjects, vector<GameObject*>* buckets[BUCKET_COLS][BUCKET_ROWS], float dt) {
	for (int i = 0; i < gameObjects.size(); ++i)
	{
		GameObject * obj = gameObjects[i];
		Vector2i curBucket = getBucket(obj->getPosition());
		obj->update(dt);
		Vector2i newBucket = getBucket(obj->getPosition());
		if (curBucket != newBucket)
		{
			bucket_remove(curBucket, obj, buckets);
			bucket_add(newBucket, obj, buckets);
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



void renderFrame(Player player) {
	window.clear(Color(0, 0, 0));
	player.draw(&window);
}

void getEvents() {
	Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();
	}
}
