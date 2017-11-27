#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include <stdlib.h>
#include <iostream>
#include "Utils.h"
#include "Player.h"
#include "Laser.h"
#include "Asteroid.h"
#include "Explosion.h"
using namespace sf;
using namespace std;

void getEvents();
bool updateState(vector<GameObject*>* gameObjects, vector<GameObject*>* buckets[BUCKET_COLS][BUCKET_ROWS], float dt);
void renderFrame(vector<GameObject*> gameObjects);
void bucket_remove(Vector2i b, GameObject* obj, vector<GameObject*>* buckets[BUCKET_COLS][BUCKET_ROWS]);
void bucket_add(GameObject* obj, vector<GameObject*>* buckets[BUCKET_COLS][BUCKET_ROWS]);
Laser* instantiateLaser(Player* player);
void createLevel(int currentLevel, vector<GameObject*>* gameObjects, vector<GameObject*>* buckets[BUCKET_COLS][BUCKET_ROWS], Player* player);
vector<GameObject*>* detect_collisions(vector<GameObject*>* gameObjects, GameObject* gameObject, Vector2i bucket, vector<GameObject*>* buckets[BUCKET_COLS][BUCKET_ROWS]);
void showMainMenu(RectangleShape startButton, RectangleShape exitButton, Text startText, Text exitText, Vector2i mousePos);
void handleMouseOver(RectangleShape* shape, Vector2i mousePos);
void handleMouseClick(RectangleShape startButton, RectangleShape exitButton, Vector2i mousePos);
void addGameObject(GameObject* gameObject, vector<GameObject*>* gameObjects, vector<GameObject*>* buckets[BUCKET_COLS][BUCKET_ROWS]);
int createLaser(vector<GameObject*>* gameObjects, vector<GameObject*>* buckets[BUCKET_COLS][BUCKET_ROWS], Player* player);

RenderWindow window;
Texture asteroidTexture;
Texture playerTexture;
Texture thrusterTexture;
Texture explosionTexture;
SoundBuffer thrusterSound;
SoundBuffer laserSound;
SoundBuffer explosionSound;
SoundBuffer winSound;
Sound soundLaser;
Sound soundExplosion;
Sound soundWin;
int currentLevel = 1;
int gameState = MAIN_MENU;
int score = 0;

int main()
{
	srand(static_cast<unsigned int>(std::time(NULL)));
	window.create(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Asteroids");
	Clock clock;
	float laserTimer = 0;
	float gameOverTimer = GAMEOVER_TIMER;

	//LOAD ASSETS
	//////////////////////////////////////////////////////
	//Sounds
	thrusterSound.loadFromFile("thruster.wav");
	laserSound.loadFromFile("laser.wav");
	explosionSound.loadFromFile("explosion.wav");
	winSound.loadFromFile("win.wav");
	soundWin.setBuffer(winSound);
	soundExplosion.setBuffer(explosionSound);
	soundLaser.setBuffer(laserSound);
	///////////////////////////////////////////////////////
	//Textures
	if (!playerTexture.loadFromFile("player.png"))
	{
		cout << "texture1 Image couldn't load!";
	}
	if (!asteroidTexture.loadFromFile("asteroid.png"))
	{
		cout << "asteroidTexture Image couldn't load!";
	}
	if (!thrusterTexture.loadFromFile("thruster.png"))
	{
		cout << "thruster Image couldn't load!";
	}
	if (!explosionTexture.loadFromFile("explosion.png"))
	{
		cout << "explosionTexture Image couldn't load!";
	}
	////////////////////////////////////////////////////////////
	//Font
	Font font;
	if (!font.loadFromFile("sansation.ttf"))
	{
		cout << "Font couldn't load!";
	}
	///////////////////////////////////////////////////////
	//UI
	Text uiText;
	uiText.setCharacterSize(20);
	uiText.setFont(font);
	uiText.setPosition((SCREEN_WIDTH / 2) - 200, SCREEN_HEIGHT - 30);
	uiText.setFillColor(Color::White);
	
	/////////////////////////////////////////////////////////////
	// GameOver
	Text gameOverText;
	gameOverText.setCharacterSize(80);
	gameOverText.setFont(font);
	gameOverText.setPosition((SCREEN_WIDTH / 2) - 200, SCREEN_HEIGHT / 2);
	gameOverText.setFillColor(Color::Red);
	gameOverText.setString("GAME OVER");

	//////////////////////////////////////////////////
	//Main Menu
	RectangleShape startButton;
	startButton.setSize(Vector2f(310, 100));
	startButton.setPosition(Vector2f(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 100));
	startButton.setFillColor(Color::Black);
	startButton.setOutlineColor(Color::White);
	startButton.setOutlineThickness(2);

	Text startText;
	startText.setCharacterSize(50);
	startText.setFont(font);
	startText.setPosition(Vector2f(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 80));
	startText.setFillColor(Color::Red);
	startText.setString("START GAME");

	RectangleShape exitButton;
	exitButton.setSize(Vector2f(310, 100));
	exitButton.setPosition(Vector2f(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 100));
	exitButton.setFillColor(Color::Black);
	exitButton.setOutlineColor(Color::White);
	exitButton.setOutlineThickness(2);

	Text exitText;
	exitText.setCharacterSize(50);
	exitText.setFont(font);
	exitText.setPosition(Vector2f(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 120));
	exitText.setFillColor(Color::Red);
	exitText.setString("EXIT GAME");
	/////////////////////////////////////////////////////

	vector<GameObject*>* buckets[BUCKET_COLS][BUCKET_ROWS];
	vector<GameObject*> gameObjects;
	Player* player = nullptr;
	while (window.isOpen())
	{
		getEvents();
		float dt = clock.restart().asSeconds();

		if (gameState == MAIN_MENU) {
			Vector2i mousePos = Mouse::getPosition(window);
			showMainMenu(startButton, exitButton, startText, exitText, mousePos);
			if (Mouse::isButtonPressed(Mouse::Button::Left)) {
				handleMouseClick(startButton, exitButton, mousePos);
			}
		}
		else if (gameState == LOADING) {		
			for (int i = 0; i < BUCKET_COLS; i++) {
				for (int j = 0; j < BUCKET_ROWS; j++) {
					buckets[i][j] = new vector<GameObject*>;
				}
			}
			if (gameObjects.size() > 0) {
				for each(GameObject* go in gameObjects) {
					delete go;
				}
			}
			gameObjects.clear();

			score = 0;
			currentLevel = 1;
			gameOverTimer = GAMEOVER_TIMER;

			player = new Player(&playerTexture, &thrusterTexture, thrusterSound);
			addGameObject(player, &gameObjects, buckets);
			createLevel(currentLevel, &gameObjects, buckets, player);

			gameState = RUN_GAME;
		}
		else if (gameState == RUN_GAME) {
		
			laserTimer += dt;
			if (Keyboard::isKeyPressed(Keyboard::Escape)) {
				gameState = MAIN_MENU;
			}
			if (Keyboard::isKeyPressed(Keyboard::Space) && laserTimer >= LASER_COOLDOWN) {
				laserTimer = createLaser(&gameObjects, buckets, player);				
			}
			if (!updateState(&gameObjects, buckets, dt)) {
				soundWin.play();
				currentLevel++;
				createLevel(currentLevel, &gameObjects, buckets, player);
			}
			renderFrame(gameObjects);
			uiText.setString("Lives: " + to_string(player->getLives()) + "      Score: " + to_string(score)+"      Level: "+to_string(currentLevel));
			window.draw(uiText);
		}
		else if (gameState == GAME_OVER) {
			if (gameOverTimer >= 0) {
				gameOverTimer -= dt;
			}
			else {
				gameState = MAIN_MENU;
			}
			window.clear();
			uiText.setString("Score: " + to_string(score));
			window.draw(uiText);
			window.draw(gameOverText);
		}
		

		window.display();
	}

	return 0;
}

int createLaser(vector<GameObject*>* gameObjects, vector<GameObject*>* buckets[BUCKET_COLS][BUCKET_ROWS], Player* player) {
	Laser* laser = instantiateLaser(player);
	addGameObject(laser, gameObjects, buckets);
	soundLaser.play();
	return 0;
}

void addGameObject(GameObject* gameObject, vector<GameObject*>* gameObjects, vector<GameObject*>* buckets[BUCKET_COLS][BUCKET_ROWS]) {
	gameObjects->push_back(gameObject);
	bucket_add(gameObject, buckets);
}

void handleMouseClick(RectangleShape startButton, RectangleShape exitButton,Vector2i mousePos) {
	if (isPointInRectangle(startButton.getPosition(), startButton.getSize(), mousePos)) {
		gameState = LOADING;
	}
	if (isPointInRectangle(exitButton.getPosition(), exitButton.getSize(), mousePos)) {
		exit(0);
	}
}

void showMainMenu(RectangleShape startButton, RectangleShape exitButton, Text startText, Text exitText, Vector2i mousePos){
	window.clear();
	handleMouseOver(&startButton, mousePos);
	handleMouseOver(&exitButton, mousePos);
	window.draw(startButton);
	window.draw(exitButton);
	window.draw(startText);
	window.draw(exitText);
}

void handleMouseOver(RectangleShape* shape, Vector2i mousePos) {
	if (isPointInRectangle(shape->getPosition(), shape->getSize(), mousePos)) {
		shape->setFillColor(Color::Yellow);
		shape->setOutlineColor(Color::Blue);
	}
	else {
		shape->setFillColor(Color::Black);
		shape->setOutlineColor(Color::White);
	}
}

void createLevel(int currentLevel, vector<GameObject*>* gameObjects, vector<GameObject*>* buckets[BUCKET_COLS][BUCKET_ROWS], Player* player) {
	for (int i = 0; i < currentLevel + 1; i++) {
		int x, y;
		do {
			x = rand() % (int)SCREEN_WIDTH;
			y = rand() % (int)SCREEN_HEIGHT;
		} while (doCirclesCollide(player->getPosition(), PLAYER_SIZE, Vector2f(x,y), ASTEROID_SIZE*ASTEROID_LIVES));
		Asteroid* asteroid = new Asteroid(&asteroidTexture, Vector2f(x, y), ASTEROID_LIVES, currentLevel);
		addGameObject(asteroid, gameObjects, buckets);
	}
}

Laser* instantiateLaser(Player* player) {
	Laser* laser = new Laser(player->getLaserPosition() + player->getFacing() * LASER_OFFSET, player->getFacing(), player->getRotation());
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
		
		//collect the new asteroids
		vector<GameObject*>* temp = detect_collisions(gameObjects, gameObject, newBucket, buckets);
		for each(GameObject* gameObject in *temp) {
			toBeAdded->push_back(gameObject);
		}
		//Remove game objects marked for deletion
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
	//add the new asteroids
	for each(GameObject* gameObject in *toBeAdded) {
		addGameObject(gameObject, gameObjects, buckets);
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
					Vector2f o1(otherObject->getPosition().x - otherObject->getLives() * ASTEROID_SIZE, otherObject->getPosition().y );
					Vector2f o2(otherObject->getPosition().x + otherObject->getLives() * ASTEROID_SIZE, otherObject->getPosition().y );
					Asteroid* a1 = new Asteroid(&asteroidTexture, o1, otherObject->getLives() - 1, currentLevel);
					Asteroid* a2 = new Asteroid(&asteroidTexture, o2, otherObject->getLives() - 1, currentLevel);
					toBeAdded->push_back(a1);
					toBeAdded->push_back(a2);
					score += KILLPOINT;
				}
				if (collision && otherObject->getLives() == 1) {
					score += KILLPOINT * 2;
				}
				if (collision) {
					Explosion* explosion = new Explosion(&explosionTexture, otherObject->getPosition(), otherObject->getLives());
					toBeAdded->push_back(explosion);
					gameObject->collide();
					otherObject->collide();
					soundExplosion.play();
				}
			}
			else if (gameObject->getType() == GOPLAYER && otherObject->getType() == GOASTEROID) {
				bool collision = false;
				collision = doCirclesCollide(gameObject->getPosition(), PLAYER_SIZE, otherObject->getPosition(), ASTEROID_SIZE * otherObject->getLives());
				if (collision) {
					Explosion* explosion = new Explosion(&explosionTexture, gameObject->getPosition(), 1);
					toBeAdded->push_back(explosion);
					gameObject->collide();
					soundExplosion.play();
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
