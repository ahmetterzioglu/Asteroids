#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include "Utils.h"
#include "Player.h"
using namespace sf;
using namespace std;

void getEvents();
void updateState(Player* player, float dt);
void renderFrame(Player player);


RenderWindow window;

int main()
{
	window.create(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Asteroids");
	Clock clock;
	Player player = Player();

	while (window.isOpen())
	{
		getEvents();
		float dt = clock.restart().asSeconds();
		updateState(&player, dt);
		renderFrame(player);
		window.display();
	}

	return 0;
}

void updateState(Player* player, float dt) {
	player->update(dt);
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
