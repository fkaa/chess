#pragma once

#include <SFML/Graphics.hpp>

#include "board.h"

class Game : public sf::Drawable {
public:
	Game();
	~Game();

	void update(sf::Window& wnd, float dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void resize(sf::Event::SizeEvent& evt);
private:
	PieceResources *resources;
	Board board;
	int turns;
};

