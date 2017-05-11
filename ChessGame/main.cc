#include <iostream>
#include <crtdbg.h>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "game.h"

int main() {
	sf::RenderWindow wnd(sf::VideoMode(720, 520), "Schack");
	sf::RenderStates state;
	
	wnd.setVerticalSyncEnabled(true);

	Game game;

	sf::Clock time;
	sf::Event evt;
	while (wnd.isOpen()) {
		while (wnd.pollEvent(evt)) {
			switch (evt.type) {
				case sf::Event::Resized:
					wnd.setView(sf::View(sf::FloatRect(0, 0, evt.size.width, evt.size.height)));
					game.resize(evt.size);
					break;
				case sf::Event::Closed:
					wnd.close();
					break;
			}
		}

		game.update(wnd, time.restart().asSeconds());

		wnd.clear();
		wnd.draw(game);
		wnd.display();
	}

	return 0;
}
