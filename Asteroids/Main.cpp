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
#include "Asteroid.h"
using namespace sf;
using namespace std;

void getEvents();
bool updateState(vector<GameObject*>* gameObjects, vector<GameObject*>* buckets[BUCKET_COLS][BUCKET_ROWS], float dt);
void renderFrame(vector<GameObject*> gameObjects);
void bucket_remove(Vector2i b, GameObject* obj, vector<GameObject*>* buckets[BUCKET_COLS][BUCKET_ROWS]);
void bucket_add(GameObject* obj, vector<GameObject*>* buckets[BUCKET_COLS][BUCKET_ROWS]);
Laser* createLaser(Player* player);
void createLevel(int currentLevel, vector<GameObject*>* gameObjects, vector<GameObject*>* buckets[BUCKET_COLS][BUCKET_ROWS]);
vector<GameObject*>* detect_collisions(vector<GameObject*>* gameObjects, GameObject* gameObject, Vector2i bucket, vector<GameObject*>* buckets[BUCKET_COLS][BUCKET_ROWS]);

RenderWindow window;
Texture asteroidTexture;
int currentLevel = 1;
int gameState = RUN_GAME;
int score = 0;
int main()
{
	srand(static_cast<unsigned int>(std::time(NULL)));
	window.create(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Asteroids");
	Clock clock;
	float laserTimer = 0;
	Texture playerTexture;
	if (!playerTexture.loadFromFile("player.png"))
	{
		cout << "texture1 Image couldn't load!";
	}
	asteroidTexture;
	if (!asteroidTexture.loadFromFile("asteroid.png"))
	{
		cout << "asteroidTexture Image couldn't load!";
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
	bucket_add(player, buckets);
	createLevel(currentLevel, &gameObjects, buckets);
	

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
	
	Text gameOverText;
	gameOverText.setCharacterSize(80);
	gameOverText.setFont(font);
	gameOverText.setPosition((SCREEN_WIDTH / 2) - 200, SCREEN_HEIGHT / 2);
	gameOverText.setFillColor(Color::Red);
	gameOverText.setString("GAME OVER");

	//////////////////////////////////////////////////

	while (window.isOpen())
	{
		getEvents();
		float dt = clock.restart().asSeconds();
		laserTimer += dt;
		if (gameState == MAIN_MENU) {
		
		}
		else if (gameState == RUN_GAME) {
		
			if (Keyboard::isKeyPressed(Keyboard::Space) && laserTimer >= LASER_COOLDOWN) {
				laserTimer = 0;
				Laser* laser = createLaser(player);
				gameObjects.push_back(laser);
				bucket_add(laser, buckets);
			}
			if (!updateState(&gameObjects, buckets, dt)) {
				createLevel(++currentLevel, &gameObjects, buckets);
			}
			renderFrame(gameObjects);
			uiText.setString("Lives: " + to_string(player->getLives()) + "      Score: " + to_string(score));
			window.draw(uiText);
		}
		else if (gameState == GAME_OVER) {
			window.clear();
			uiText.setString("Score: " + to_string(player->getLives()));
			window.draw(uiText);
			window.draw(gameOverText);
		}
		

		window.display();
	}

	return 0;
}

void createLevel(int currentLevel, vector<GameObject*>* gameObjects, vector<GameObject*>* buckets[BUCKET_COLS][BUCKET_ROWS]) {
	for (int i = 0; i < currentLevel + 1; i++) {
		int x = rand() % 1024;
		int y = rand() % 768;
		Asteroid* asteroid = new Asteroid(&asteroidTexture, Vector2f(x, y), ASTEROID_LIVES, currentLevel);
		gameObjects->push_back(asteroid);
		bucket_add(asteroid, buckets);
	}
}

Laser* createLaser(Player* player) {
	Laser* laser = new Laser(player->getPosition() + player->getFacing() * LASER_OFFSET, player->getFacing(), player->getRotation());
	return laser;
}

bool updateState(vector<GameObject*>* gameObjects, vector<GameObject*>* buckets[BUCKET_COLS][BUCKET_ROWS], float dt) {
	bool hasAsteroidsLeft = false;
	vector<GameObject*>* toBeAdded = new vector<GameObject*>;
	for (int i = 0; i < gameObjects->size();)
	{
		GameObject* gameObject = *(gameObjects->begin() + i);
		if (gameObject->getType() == GOASTEROID) {
			hasAsteroidsLeft = true;
		}
		Vector2i curBucket = getBucket(gameObject->getPosition());
		gameObject->update(dt);
		Vector2i newBucket = getBucket(gameObject->getPosition());
		if (curBucket != newBucket)
		{
			bucket_remove(curBucket, gameObject, buckets);
			bucket_add(gameObject, buckets);
		}
		
		vector<GameObject*>* temp = detect_collisions(gameObjects, gameObject, newBucket, buckets);
		for each(GameObject* gameObject in *temp) {
			toBeAdded->push_back(gameObject);
		}
		if (gameObject->isToBeDestroyed()) {
			gameObjects->erase(gameObjects->begin() + i);
			bucket_remove(newBucket, gameObject, buckets);
			delete gameObject;
			gameObject = nullptr;
			continue;
		}
		i++;
		delete temp;
		temp = nullptr;
	}
	for each(GameObject* gameObject in *toBeAdded) {
		gameObjects->push_back(gameObject);
		bucket_add(gameObject, buckets);
	}
	
	delete toBeAdded;
	toBeAdded = nullptr;
	return hasAsteroidsLeft;
}

vector<GameObject*>* detect_collisions(vector<GameObject*>* gameObjects, GameObject* gameObject, Vector2i bucket, vector<GameObject*>* buckets[BUCKET_COLS][BUCKET_ROWS]) {
	vector<GameObject*>* toBeAdded = new vector<GameObject*>;
	vector<Vector2i> possibleBuckets;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			int x = (bucket.x + i);
			int y = (bucket.y + j);
			if (x >= 0 && x < BUCKET_COLS && y >= 0 && y < BUCKET_ROWS) {

				possibleBuckets.push_back(Vector2i(x, y));		
			}
		}
	}
	for each(Vector2i bucket in possibleBuckets) {
		for each(GameObject* otherObject in *buckets[bucket.x][bucket.y]) {
			if (otherObject == gameObject) {
				continue;
			}
			if (gameObject->getType() == GOASTEROID && otherObject->getType()==GOASTEROID) {
				bool collision = false;
				collision = doCirclesCollide(gameObject->getPosition(), ASTEROID_SIZE * gameObject->getLives(), otherObject->getPosition(), ASTEROID_SIZE * otherObject->getLives());
				if (collision) {	
					Vector2f direction = normalizeVector(gameObject->getPosition() - otherObject->getPosition());
					gameObject->setVelocity(direction);
					otherObject->setVelocity(-direction);
				}
			}
			else if (gameObject->getType() == GOLASER && otherObject->getType() == GOASTEROID) {
				bool collision = false;
				collision = doesCircleAndPointCollide(otherObject->getPosition(), ASTEROID_SIZE * otherObject->getLives(), gameObject->getPosition());
				if (collision && otherObject->getLives() > 1) {
					Vector2f o1(otherObject->getPosition().x, otherObject->getPosition().y );
					Vector2f o2(otherObject->getPosition().x + 2 * otherObject->getLives() * ASTEROID_SIZE, otherObject->getPosition().y );
					Asteroid* a1 = new Asteroid(&asteroidTexture, o1, otherObject->getLives() - 1, currentLevel);
					Asteroid* a2 = new Asteroid(&asteroidTexture, o2, otherObject->getLives() - 1, currentLevel);
					toBeAdded->push_back(a1);
					toBeAdded->push_back(a2);
					gameObject->collide();
					otherObject->collide();
				}
				if (collision && otherObject->getLives() == 1) {
					gameObject->collide();
					otherObject->collide();
				}
			}
			else if (gameObject->getType() == GOPLAYER && otherObject->getType() == GOASTEROID) {
				bool collision = false;
				collision = doCirclesCollide(gameObject->getPosition(), PLAYER_SIZE, otherObject->getPosition(), ASTEROID_SIZE * otherObject->getLives());
				if (collision) {
					gameObject->collide();
				}
				if (gameObject->getLives()==0) {
					gameState = GAME_OVER;
					return toBeAdded;
				}
			}
		}
	}
	return toBeAdded;
}

void bucket_add(GameObject* obj, vector<GameObject*>* buckets[BUCKET_COLS][BUCKET_ROWS])
{
	Vector2i bucket = getBucket(obj->getPosition());
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
